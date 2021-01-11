#include "invoker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int destroyFifo(t_fifo fifo)
{
    unlink(fifo);
}

/**
 * \return 0 if it was a success
 */
int createDeamon(t_fifo fifo)
{
    if (fork() == 0)
    {
        // son process
        printf("\x1B[0m[Invoker] Create deamon\n");
        execlp("../deamon/deamon", "deamon", fifo, NULL);
    }
    if (mkfifo(fifo, FIFO_MODE) != 0)
    {
        unlink(fifo);
        if (mkfifo(fifo, FIFO_MODE) != 0)
        {
            perror("[ERROR] fifo creation");
        }
    }
}

void pingDeamon(t_fifo fifo)
{
    send(fifo, "PING");
}

int isDeamonAlive(t_fifo fifo)
{
    pingDeamon(fifo);
    char buffer[BUFFER_SIZE];
    int ret = listenWithTimeout(fifo, buffer);

    printf("\x1B[0m[Invoker] Recieved [%s]\n", buffer);
    printf("\x1B[0m[Invoker] Exit code [%d]\n", ret);
}

int main(int argc, char const *argv[])
{
    t_fifo myFifo = "/tmp/sophia";

    createDeamon(myFifo);
    isDeamonAlive(myFifo);
    return 0;
}
