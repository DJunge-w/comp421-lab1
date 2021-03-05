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

#define buflen1_1 2
char *read1_1;

#define buflen2_1 2
char *read2_1;

#define buflen3_1 2
char *read3_1;

#define buflen4_1 2
char *read4_1;

#define buflen1_2 3
char *read1_2;

#define buflen2_2 3
char *read2_2;

#define buflen3_2 3
char *read3_2;

#define buflen4_2 3
char *read4_2;

struct termstat *stats;

void reader1_1(void *);
void reader2_1(void *);
void reader3_1(void *);
void reader4_1(void *);

void reader1_2(void *);
void reader2_2(void *);
void reader3_2(void *);
void reader4_2(void *);

void readstats(void *);

char string1_1[] = "abcdefghijklmnopqrstuvwxyz\n";
int length1_1 = sizeof(string1_1) - 1;

char string2_1[] = "abcdefghijklmnopqrstuvwxyz\n";
int length2_1 = sizeof(string2_1) - 1;

char string3_1[] = "abcdefghijklmnopqrstuvwxyz\n";
int length3_1 = sizeof(string3_1) - 1;

char string4_1[] = "abcdefghijklmnopqrstuvwxyz\n";
int length4_1 = sizeof(string4_1) - 1;

char string1_2[] = "0123456789\n";
int length1_2 = sizeof(string1_2) - 1;

char string2_2[] = "0123456789\n";
int length2_2 = sizeof(string2_2) - 1;

char string3_2[] = "0123456789\n";
int length3_2 = sizeof(string3_2) - 1;

char string4_2[] = "0123456789\n";
int length4_2 = sizeof(string4_2) - 1;

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

    read1_1 = malloc(sizeof(char)*buflen1_1);
    read2_1 = malloc(sizeof(char)*buflen2_1);
    read3_1 = malloc(sizeof(char)*buflen3_1);
    read4_1 = malloc(sizeof(char)*buflen4_1);
    read1_2 = malloc(sizeof(char)*buflen1_2);
    read2_2 = malloc(sizeof(char)*buflen2_2);
    read3_2 = malloc(sizeof(char)*buflen3_2);
    read4_2 = malloc(sizeof(char)*buflen4_2);

    stats = malloc((sizeof(struct termstat))*4);
    int i;
    for (i = 0; i < 4; i++) {
        stats[i] = (struct termstat){0, 0, 0, 0};
    }
    sleep(40);

    ThreadCreate(writer1_1, NULL);
    ThreadCreate(reader1_1, NULL);
    ThreadCreate(readstats, NULL);
    ThreadCreate(writer1_2, NULL);
    ThreadCreate(reader1_2, NULL);
    ThreadCreate(writer2_1, NULL);
    ThreadCreate(reader2_1, NULL);
    ThreadCreate(writer2_2, NULL);
    ThreadCreate(reader2_2, NULL);
    ThreadCreate(writer3_1, NULL);
    ThreadCreate(reader3_1, NULL);
    ThreadCreate(writer3_2, NULL);
    ThreadCreate(reader3_2, NULL);
    ThreadCreate(writer4_1, NULL);
    ThreadCreate(reader4_1, NULL);
    ThreadCreate(writer4_2, NULL);
    ThreadCreate(reader4_2, NULL);
    ThreadCreate(readstats, NULL);
    sleep(40);
    ThreadCreate(readstats, NULL);
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
    status = WriteTerminal(0, string1_2, length1_2);
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
    status = WriteTerminal(2, string3_2, length3_2);
    if (status != length3_2)
        fprintf(stderr, "Error: writer2 status = %d, length2 = %d\n",
                status, length3_2);
}

void
writer4_2(void *arg)
{
    (void) arg;
    int status;
    status = WriteTerminal(3, string4_2, length4_2);
    if (status != length4_2)
        fprintf(stderr, "Error: writer2 status = %d, length2 = %d\n",
                status, length4_2);
}

void
reader1_1(void *arg)
{
    (void) arg;
    int status;
    status = ReadTerminal(0, read1_1, buflen1_1);
    fprintf(stdout, "readed buf = %s, buflen1_1 = %d, status = %d\n",
                read1_1, buflen1_1, status);

}

void
reader2_1(void *arg)
{
    (void) arg;
    int status;
    status = ReadTerminal(1, read2_1, buflen2_1);
    fprintf(stdout, "readed buf = %s, buflen2_1 = %d, status = %d\n",
            read2_1, buflen2_1, status);

}

void
reader3_1(void *arg)
{
    (void) arg;
    int status;
    status = ReadTerminal(2, read3_1, buflen3_1);
    fprintf(stdout, "readed buf = %s, buflen3_1 = %d, status = %d\n",
            read3_1, buflen3_1, status);

}

void
reader4_1(void *arg)
{
    (void) arg;
    int status;
    status = ReadTerminal(3, read4_1, buflen4_1);
    fprintf(stdout, "readed buf = %s, buflen4_1 = %d, status = %d\n",
            read4_1, buflen4_1, status);

}

void
reader1_2(void *arg)
{
    (void) arg;
    int status;
    status = ReadTerminal(0, read1_2, buflen1_2);
    fprintf(stdout, "readed buf = %s, buflen1_2 = %d, status = %d\n",
            read1_2, buflen1_2, status);

}

void
reader2_2(void *arg)
{
    (void) arg;
    int status;
    status = ReadTerminal(1, read2_2, buflen2_2);
    fprintf(stdout, "readed buf = %s, buflen2_2 = %d, status = %d\n",
            read2_2, buflen2_2, status);

}

void
reader3_2(void *arg)
{
    (void) arg;
    int status;
    status = ReadTerminal(2, read3_2, buflen3_2);
    fprintf(stdout, "readed buf = %s, buflen3_2 = %d, status = %d\n",
            read3_2, buflen3_2, status);

}

void
reader4_2(void *arg)
{
    (void) arg;
    int status;
    status = ReadTerminal(3, read4_2, buflen4_2);
    fprintf(stdout, "readed buf = %s, buflen4_2 = %d, status = %d\n",
            read4_2, buflen4_2, status);

}

void
readstats(void *arg)
{
    (void) arg;
    TerminalDriverStatistics(stats);
    int i;
    for (i = 0; i < 4; i++) {
        printf("stats %d term, tty_in %d, tty_out %d, user_in %d, user_out %d\n",
               i, stats[i].tty_in, stats[i].tty_out, stats[i].user_in, stats[i].user_out);
    }
}