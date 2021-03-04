#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <terminals.h>
#include <unistd.h>

void reader(void *);

#define buflen 10
char buf[buflen];

int
main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    InitTerminalDriver();
    InitTerminal(1);

    ThreadCreate(reader, NULL);

    ThreadWaitAll();

    exit(0);
}

void
reader(void *arg)
{
    (void) arg;
    int status;
    sleep(20);
    printf("Doing ReadTerminal... '");
    fflush(stdout);
    status = ReadTerminal(1, buf, buflen);
    printf("'. Done: status = %d.\n", status);
    printf("%s\n", buf);
    fflush(stdout);
}
