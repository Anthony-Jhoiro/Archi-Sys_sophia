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
    send(fifo, "PONG");
}

int listening(t_fifo fifo)
{
    char buffer[BUFFER_SIZE];

    int deamonIsListening = 1;

    printf("\x1B[34m[DEAMON] Listening for inputs...\n");
    while (deamonIsListening)
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
            pongInvoker(fifo);
        }
        else if (strcmp(buffer, "KILL") == 0)
        {
            printf("\x1B[34m[DEAMON] Stoping the deamon\n");
            deamonIsListening = 0;
        }
        close(fd);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "One argument is needed\n");
        return 1;
    }
    printf("\x1B[34m[Deamon] Hello !\n");

    // Get fifo name from the fist argument
    t_fifo myFifo = argv[1];

    printf("\x1B[34m[Deamon] Using fifo [%s]\n", myFifo);

    return listening(myFifo);
}
