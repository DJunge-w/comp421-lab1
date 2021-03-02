#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <hardware.h>
#include <terminals.h>

/*
 * A condition variable for each terminal's input register to wait on.  Declared
 * 'static' as with all variables that should only be seen inside
 * this monitor.
 */
static cond_id_t inputcond[4];

/*
 * A condition variable for each terminal's output regitser to wait on.
 */
static cond_id_t outputcond[4];


/*
 * Require procedures for hardwares
 */
extern void
ReceiveInterrupt(int term)
{
    (void) term;
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
    (void) term;
    return 0;
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
    return 0;
}
