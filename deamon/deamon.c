#include "deamon.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

void pongInvoker(t_fifo fifo)
{
    printf("\x1B[34m[DEAMON] Send pong\n");
    send(fifo, "PONG");
    printf("\x1B[34m[DEAMON] pong sent\n");
}

void listening(t_fifo fifo)
{

    char buffer[BUFFER_SIZE];

    printf("\x1B[34m[DEAMON] Listening...\n");
    while (1)
    {
        int fd = open(fifo, O_RDONLY);
        FILE *fp = fdopen(fd, "r");

        // int ret = read(fd, buffer, BUFFER_SIZE);

        // printf("Ret %d", ret);
        fscanf(fp, "%s", buffer);

        if (strlen(buffer) == 0)
            continue;

        printf("\x1B[34m[DEAMON] recieved [%s]\n", buffer);
        sleep(1);

        if (strcmp(buffer, "PING") == 0)
        {
            printf("\x1B[34m[DEAMON] PING recieved\n");

            pongInvoker(fifo);
        }
        close(fd);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "One argument is needed\n");
        return 1;
    }

    // Get fifo name from the fist argument
    t_fifo myFifo = argv[1];

    printf("\x1B[34m[Deamon] Using fifo [%s]\n", myFifo);

    listening(myFifo);

    return 0;
}
