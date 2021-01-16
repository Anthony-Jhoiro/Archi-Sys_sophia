#include "invoker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

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
        invokerSay("Create deamon");
        execlp("../deamon/deamon", "deamon", fifo, NULL);
    }
    return forceFifoCreation(fifo);
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

    printf("=> %d [%s] \n", ret, buffer);

    if (ret != 0)
        return ret;

    if (strcmp("PONG", buffer) != 0)
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
    destroyFifo(fifo);
    return sendKill;
}

int cmdState(t_fifo fifo)
{
    invokerSay("Controling deamon state");
    if (isDeamonAlive(fifo) == 0)
    {
        invokerSay("Deamon is alive");
        return 0;
    }
    invokerSay("Deamon is not alive");
    return 1;
}

int cmdStart(t_fifo fifo)
{
    invokerSay("Starting deamon");
    if (createDeamon(fifo) == 0)
    {
        invokerSay("Success Starting deamon");
        return 0;
    }
    invokerSay("Fail Starting deamon");
    return 1;
}

int cmdStop(t_fifo fifo)
{
    invokerSay("Killing the deamon");
    if (killDeamon(fifo) == 0)
    {
        invokerSay("Success Killing the deamon");
        return 0;
    }
    invokerSay("Fail Killing the deamon");
    return 1;
}

int cmdRestart(t_fifo fifo)
{
    invokerSay("Restarting the deamon");
    cmdStop(fifo);
    return cmdStart(fifo);
}

int cmdDate(t_fifo fifo)
{
    if (!isFifoOpen(fifo))
    {
        return -1;
    }
    int sendSuccess = send(fifo, "TIME");
    if (sendSuccess != 0)
        return 1;

    char buffer[BUFFER_SIZE];

    int ret = listenWithTimeout(fifo, buffer);

    if (ret != 0)
        return ret;

    invokerSay("Date : %s s", buffer);

    time_t time = strtol(buffer, NULL, 10);

    return 0;
}

int cmdDuration(t_fifo fifo)
{
    if (!isFifoOpen(fifo))
    {
        return -1;
    }
    int sendSuccess = send(fifo, "DURATION");
    if (sendSuccess != 0)
        return 1;

    char buffer[BUFFER_SIZE];

    int ret = listenWithTimeout(fifo, buffer);

    if (ret != 0)
        return ret;

    invokerSay("Duration : %s s", buffer);

    time_t time = strtol(buffer, NULL, 10);

    return 0;
}

int cmdReset(t_fifo fifo)
{
    if (!isFifoOpen(fifo))
    {
        return -1;
    }
    return send(fifo, "RESET");
}

int main(int argc, char *argv[])
{
    t_fifo myFifo = "/tmp/sophia";

    if (argc == 1)
    {
        invokerSay("Error, one argument is needed");
        return 1;
    }

    char *command = argv[1];

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
        return cmdDate(myFifo);
    }
    else if (strcmp(command, "--duration") == 0)
    {
        return cmdDuration(myFifo);
    }
    else if (strcmp(command, "--reset") == 0)
    {
        return cmdReset(myFifo);
    }
    else if (strcmp(command, "--stop") == 0)
    {
        return cmdStop(myFifo);
    }
    else if (strcmp(command, "--restart") == 0)
    {
        return cmdRestart(myFifo);
    }
    else
    {
        fprintf(stderr, "Unknown argument [%s]\n", command);
    }

    return 0;
}
