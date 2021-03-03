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
 * A single echo buffer
 */
static char * buffer;
static 

/*
 * Require procedures for hardwares
 */
extern void
ReceiveInterrupt(int term)
{
    char received = ReadDataRegister(term);
    WriteDataRegister(term, receive);
}

extern void
TransmitInterrupt(int term)
{
    (void) term;
}

extern int
WriteTerminal(int term, char *buf, int buflen)
{
    (void) term;
    (void) buf;
    (void) buflen;
    return 0;
}

extern int
ReadTerminal(int term, char *buf, int buflen)
{
    (void) term;
    (void) buf;
    (void) buflen;
    return 0;
}

extern int
InitTerminal(int term)
{
    return InitHardware(term);
}

extern int
TerminalDriverStatistics(struct termstat *stats)
{
    (void) stats;
    return 0;
}

extern int
InitTerminalDriver()
{
    (void) inputcond;
    (void) outputcond;
    buffer = malloc(sizeof(char)*1000);
    return 0;
}
