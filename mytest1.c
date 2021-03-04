#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <terminals.h>

void reader(void *);

int buflen = 10;
char buf[] = malloc(sizeof(char)*10)

int main(int argc, char **argv)
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
    int status;
    sleep(20);
    printf("Doing ReadTerminal... '");
    fflush(stdout);
    status = ReadTerminal(1, &buf, buflen);
    printf("'. Done: status = %d.\n", status);
    fflush(stdout);
}
