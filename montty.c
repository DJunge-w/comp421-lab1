#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <hardware.h>
#include <terminals.h>

/*
 * echo Buffer size
 */
#define SIZE_OF_ECHO_BUFFER 1024
#define SIZE_OF_INPUT_BUFFER 10
#define FAILED -1
#define SUCCESS 0

/*
 * state indicating whether output loop is establish
 */
static int echoing = FAILED;
/*
 * Number of reader waiting
 */
static int waitingreaders = 0;
/*
 * state whether other reader is reading
 */
static int reading = FAILED;
/*
 * number of line breaks in the input buffer
 */
static int linebreaks = 0;

/*
 * A condition variable for each readTerminal to wait on, only one reader
 * at a time.
 */
static cond_id_t read;

/*
 * A condition variable for each readTerminal to wait on, only afer a line
 * terminated by new line charater can one reader return.
 */
static cond_id_t linecompleted;

/*
 * character buffer data structure
 */
typedef struct {
    size_t in;
    size_t out;
    size_t count;
    size_t size;
    char* data;
} queue_t;

static char dequeue(queue_t *queue) {
    if (queue->count == 0) {
        return (char) 0;
    }
    char output = queue->data[queue->out];
    queue->data[queue->out] = (char) 0;
    queue->count--;
    queue->out = (queue->out + 1) % queue->size;
    return output;
}

static int enqueue(queue_t *queue, char input) {
    if (queue->count == queue->size) {
        return FAILED;
    }
    queue->data[queue->in] = input;
    queue->in = (queue->in + 1) % queue->size;
    queue->count++;
    return SUCCESS;
}

/*
 * A single echo buffer
 */
static queue_t echoBuffer;

/*
 * A single input buffer
 */
static queue_t inputBuffer;

static void
enqueue_echo(int term, char received)
{
    if (echoing == FAILED)
    {
        //not in the write and transmit interrupt loop
        //send the first WriteDataRegister call
        if (received == '\r') {
            WriteDataRegister(term, '\r');
            enqueue(&echoBuffer, '\n');
        } else if (received == '\n') {
            WriteDataRegister(term, '\r');
            enqueue(&echoBuffer, '\n');
        } else if (received == '\b' || received == '\177') {
            WriteDataRegister(term, '\b');
            enqueue(&echoBuffer, ' ');
            enqueue(&echoBuffer, '\b');
        } else {
            //any other character
            WriteDataRegister(term, received);
        }
        echoing = SUCCESS;
    } else {
        if (received == '\r') {
            enqueue(&echoBuffer, '\r');
            enqueue(&echoBuffer, '\n');
        } else if (received == '\n') {
            enqueue(&echoBuffer, '\r');
            enqueue(&echoBuffer, '\n');
        } else if (received == '\b' || received == '\177') {
            enqueue(&echoBuffer, '\b');
            enqueue(&echoBuffer, ' ');
            enqueue(&echoBuffer, '\b');
        } else {
            //any other character
            enqueue(&echoBuffer, received);
        }
    }
}

/*
 * enqueue input buffer
 */
static int
enqueue_input(int term, char received)
{
    (void) term;
    int inputstatus;
    //char process before enqueue
    if (received == '\r') {
        inputstatus = enqueue(&inputBuffer, '\n');
        if (inputstatus == SUCCESS) {
            //enqueue success
            linebreaks++;
            printf("receive linebreaks %d\n", linebreaks);
            fflush(stdout);
            if (linebreaks == 1) {
                //signal readers that the line is completed.
                CondSignal(linecompleted);
            }
            return inputstatus;
        } else {
            //queue full, enqueue failed
            return inputstatus;
        }

    } else if (received == '\n') {
        inputstatus = enqueue(&inputBuffer, received);
        if (inputstatus == SUCCESS) {
            //enqueue success
            linebreaks++;
            printf("receive linebreaks %d\n", linebreaks);
            fflush(stdout);
            if (linebreaks == 1) {
                //signal readers that the line is completed.
                CondSignal(linecompleted);
            }
            return inputstatus;
        } else {
            //queue full, enqueue failed
            return inputstatus;
        }
    } else if (received == '\b' || received == '\177') {
        //remove one character from input buffer
        if (inputBuffer.count > 0) {
            dequeue(&inputBuffer);
            return SUCCESS;
        }
        return FAILED;
        //empty input buffer, do nothing
    } else {
        //any other charater
        inputstatus = enqueue(&inputBuffer, received);
        return inputstatus;
    }
}

/*
 * Require procedures for hardwares
 */
extern void
ReceiveInterrupt(int term)
{
    Declare_Monitor_Entry_Procedure();
    char received = ReadDataRegister(term);
    //add received to input buffer
    int input_status = enqueue_input(term, received);
    //add received to echo buffer
    if (input_status == FAILED) {
        //when input buffer is full
        enqueue_echo(term, '\a');
    } else {
        enqueue_echo(term, received);
    }

}

extern void
TransmitInterrupt(int term)
{
    Declare_Monitor_Entry_Procedure();
    if (echoBuffer.count > 0) {
        char received = dequeue(&echoBuffer);
        WriteDataRegister(term, received);
    } else {
        //nothing to output
        echoing = FAILED;
    }
}

extern int
WriteTerminal(int term, char *buf, int buflen)
{
    Declare_Monitor_Entry_Procedure();
    (void) term;
    (void) buf;
    (void) buflen;
    return 0;
}

extern int
ReadTerminal(int term, char *buf, int buflen)
{
    Declare_Monitor_Entry_Procedure();
    (void) term;
    //case buflen = 0, nothing to read
    if (buflen == 0) {
        return 0;
    }
    waitingreaders++;
    while (reading == SUCCESS){
        //Wait for other reader to complete
        CondWait(read);
    }
    printf("%s\n", "reader wait success");
    fflush(stdout);
    //Current reader is reading
    waitingreaders--;
    reading = SUCCESS;
    printf("linebreaks %d\n", linebreaks);
    fflush(stdout);
    while (linebreaks <= 0) {
        //wait for the line to complete
        CondWait(linecompleted);
    }
    printf("%s\n", "line completed");
    fflush(stdout);
    //read the line
    int count = 0;
    while (count < buflen) {
        char curr = dequeue(&inputBuffer);
        buf[count] = curr;
        count++;
        if (curr == '\n') {
            linebreaks--;
            break;
        }
    }
    //completed reading
    reading = FAILED;
    //wake up other readers
    if (waitingreaders > 0) {
        CondSignal(read);
    }
    return count;
}

extern int
InitTerminal(int term)
{
    Declare_Monitor_Entry_Procedure();
    return InitHardware(term);
}

extern int
TerminalDriverStatistics(struct termstat *stats)
{
    Declare_Monitor_Entry_Procedure();
    (void) stats;
    return 0;
}

extern int
InitTerminalDriver()
{
    Declare_Monitor_Entry_Procedure();
    read = CondCreate();
    linecompleted = CondCreate();
    //Initialize buffers
    echoBuffer = (queue_t){0, 0, 0, SIZE_OF_ECHO_BUFFER, malloc(sizeof(char)*SIZE_OF_ECHO_BUFFER)};
    inputBuffer = (queue_t){0, 0, 0, SIZE_OF_INPUT_BUFFER, malloc(sizeof(char)*SIZE_OF_ECHO_BUFFER)};
    return 0;
}
