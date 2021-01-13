#include "invoker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int destroyFifo(t_fifo fifo)
{
    unlink(fifo);
}

/**
 * \return 0 if it was a success
 */
int createDeamon(t_fifo fifo)
{
    pid_t deamonProcess = fork();
    if (deamonProcess == -1)
    {
        perror("Error creating the deamon");
        return 1;
    }
    else if (deamonProcess == 0)
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

int pingDeamon(t_fifo fifo)
{
    return send(fifo, "PING");
}

int isDeamonAlive(t_fifo fifo)
{
    if (!isFifoOpen(fifo))
    {
        return -1;
    }
    int pingSuccess = pingDeamon(fifo);
    if (pingSuccess != 0)
        return 1;

    char buffer[BUFFER_SIZE];

    int ret = listenWithTimeout(fifo, buffer);

    if (ret != 0)
        return ret;

    if (strcmp("PONG\n", buffer) != 0)
        return 1;

    return 0;
}

int killDeamon(t_fifo fifo)
{
    if (!isFifoOpen(fifo))
    {
        return -1;
    }
    int sendKill = send(fifo, "KILL");
    unlink(fifo);
    return sendKill;
}

int cmdState(t_fifo fifo)
{
    printf("\x1B[0m[Invoker] Controling deamon state\n");
    if (isDeamonAlive(fifo) == 0)
    {
        printf("\x1B[0m[Invoker] Deamon is alive\n");
        return 0;
    }
    printf("\x1B[0m[Invoker] Deamon is not alive\n");
    return 1;
}

int cmdStart(t_fifo fifo)
{
    printf("\x1B[0m[Invoker] Starting deamon\n");
    if (createDeamon(fifo) == 0)
    {
        printf("\x1B[0m[Invoker] Success Starting deamon\n");
        return 0;
    }
    printf("\x1B[0m[Invoker] Fail Starting deamon\n");
    return 1;
}

int cmdStop(t_fifo fifo)
{
    printf("\x1B[0m[Invoker] Killing the deamon\n");
    if (killDeamon(fifo) == 0)
    {
        printf("\x1B[0m[Invoker] Success Killing the deamon\n");
        return 0;
    }
    printf("\x1B[0m[Invoker] Fail Killing the deamon\n");
    return 1;
}

int main(int argc, char *argv[])
{
    t_fifo myFifo = "/tmp/sophia";

    if (argc == 1)
    {
        puts("Error, one argument is needed");
        return 1;
    }

    char *command = argv[1];
    printf("\x1B[0m[Invoker] Commande = %s\n", command);

    if (strcmp(command, "--start") == 0)
    {
        return cmdStart(myFifo);
    }
    else if (strcmp(command, "--state") == 0)
    {
        return cmdState(myFifo);
    }
    else if (strcmp(command, "--help") == 0)
    {
        fprintf(stderr, "Not implemented.\n");
    }
    else if (strcmp(command, "--date") == 0)
    {
        fprintf(stderr, "Not implemented.\n");
    }
    else if (strcmp(command, "--duration") == 0)
    {
        fprintf(stderr, "Not implemented.\n");
    }
    else if (strcmp(command, "--reset") == 0)
    {
        fprintf(stderr, "Not implemented.\n");
    }
    else if (strcmp(command, "--stop") == 0)
    {
        return cmdStop(myFifo);
    }
    else if (strcmp(command, "--restart") == 0)
    {
        fprintf(stderr, "Not implemented.\n");
    }
    else
    {
        fprintf(stderr, "\x1B[0m[Invoker] Unknown argument [%s]\n", command);
    }

    return 0;
}
