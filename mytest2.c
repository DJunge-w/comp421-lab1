#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <terminals.h>
#include <unistd.h>

void writer1_1(void *);
void writer2_1(void *);
void writer3_1(void *);
void writer4_1(void *);

void writer1_2(void *);
void writer2_2(void *);
void writer3_2(void *);
void writer4_2(void *);

char string1_1[] = "abcdefghijklmnopqrstuvwxyz\n";
int length1_1 = sizeof(string1_1) - 1;

char string2_1[] = "abcdefghijklmnopqrstuvwxyz\n";
int length2_1 = sizeof(string2_1) - 1;

char string3_1[] = "abcdefghijklmnopqrstuvwxyz\n";
int length3_1 = sizeof(string3_1) - 1;

char string4_1[] = "abcdefghijklmnopqrstuvwxyz\n";
int length4_1 = sizeof(string4_1) - 1;

char string1_2[] = "0123456789\n";
int length2 = sizeof(string1_2) - 1;

char string2_2[] = "0123456789\n";
int length2 = sizeof(string2_2) - 1;

char string3_2[] = "0123456789\n";
int length2 = sizeof(string3_2) - 1;

char string4_2[] = "0123456789\n";
int length2 = sizeof(string4_2) - 1;

int
main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    InitTerminalDriver();
    InitTerminal(0);
    InitTerminal(1);
    InitTerminal(2);
    InitTerminal(3);
    sleep(40);

    ThreadCreate(writer1_1, NULL);
    ThreadCreate(writer1_2, NULL);
    ThreadCreate(writer2_1, NULL);
    ThreadCreate(writer2_2, NULL);
    ThreadCreate(writer3_1, NULL);
    ThreadCreate(writer3_2, NULL);
    ThreadCreate(writer4_1, NULL);
    ThreadCreate(writer4_2, NULL);

    ThreadWaitAll();
    sleep(20);
    exit(0);
}

void
writer1_1(void *arg)
{
    (void) arg;
    int status;
    status = WriteTerminal(0, string1_1, length1_1);
    if (status != length1_1)
        fprintf(stderr, "Error: writer1 status = %d, length1 = %d\n",
                status, length1_1);
}

void
writer2_1(void *arg)
{
    (void) arg;
    int status;
    status = WriteTerminal(1, string2_1, length2_1);
    if (status != length2_1)
        fprintf(stderr, "Error: writer1 status = %d, length1 = %d\n",
                status, length2_1);
}

void
writer3_1(void *arg)
{
    (void) arg;
    int status;
    status = WriteTerminal(2, string3_1, length3_1);
    if (status != length3_1)
        fprintf(stderr, "Error: writer1 status = %d, length1 = %d\n",
                status, length3_1);
}

void
writer4_1(void *arg)
{
    (void) arg;
    int status;
    status = WriteTerminal(3, string4_1, length4_1);
    if (status != length4_1)
        fprintf(stderr, "Error: writer1 status = %d, length1 = %d\n",
                status, length4_1);
}

void
writer1_2(void *arg)
{
    (void) arg;
    int status;
    status = WriteTerminal(1, string1_2, length1_2);
    if (status != length1_2)
        fprintf(stderr, "Error: writer2 status = %d, length2 = %d\n",
                status, length1_2);
}

void
writer2_2(void *arg)
{
    (void) arg;
    int status;
    status = WriteTerminal(1, string2_2, length2_2);
    if (status != length2_2)
        fprintf(stderr, "Error: writer2 status = %d, length2 = %d\n",
                status, length2_2);
}

void
writer3_2(void *arg)
{
    (void) arg;
    int status;
    status = WriteTerminal(1, string3_2, length3_2);
    if (status != length3_2)
        fprintf(stderr, "Error: writer2 status = %d, length2 = %d\n",
                status, length3_2);
}

void
writer4_2(void *arg)
{
    (void) arg;
    int status;
    status = WriteTerminal(1, string4_2, length4_2);
    if (status != length4_2)
        fprintf(stderr, "Error: writer2 status = %d, length2 = %d\n",
                status, length4_2);
}