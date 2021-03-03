#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <hardware.h>
#include <terminals.h>

/*
 * echo Buffer size
 */
#define SIZE_OF_ECHO_BUFFER 1024
#define FAILED -1
#define SUCCESS 0

static int echoing = FAILED;
/*
 * A condition variable for each terminal's input register to wait on.  Declared
 * 'static' as with all variables that should only be seen inside
 * this monitor.
 */
static cond_id_t inputcond[NUM_TERMINALS];

/*
 * A condition variable for each terminal's output regitser to wait on.
 */
static cond_id_t outputcond[NUM_TERMINALS];

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
 * Require procedures for hardwares
 */
extern void
ReceiveInterrupt(int term)
{
    Declare_Monitor_Entry_Procedure();
    char received = ReadDataRegister(term);
    if (echoing == FAILED)
    {
        //not in the write and transmit interrupt loop
        WriteDataRegister(term, received);
        echoing = SUCCESS;
    } else {
        enqueue(&echoBuffer, received);
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
    (void) buf;
    (void) buflen;
    return 0;
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
    (void) inputcond;
    (void) outputcond;
    echoBuffer = (queue_t){0, 0, 0, SIZE_OF_ECHO_BUFFER, malloc(sizeof(char)*SIZE_OF_ECHO_BUFFER)};
    return 0;
}
