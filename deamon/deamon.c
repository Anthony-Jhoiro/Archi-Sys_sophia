#include "deamon.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

void pongInvoker(t_fifo fifo)
{
    send(fifo, "PONG");
}

int listening(t_fifo fifo)
{
    char buffer[BUFFER_SIZE];

    time_t baseTime = time(0);

    int deamonIsListening = 1;

    deamonSay("Listening for inputs...");
    while (deamonIsListening)
    {
        int fd = open(fifo, O_RDONLY);
        FILE *fp = fdopen(fd, "r");
        fscanf(fp, "%s", buffer); // We are using fscanf here to pause the process unitil the next input

        if (strlen(buffer) == 0)
            continue;

        deamonSay("recieved [%s]", buffer);
        usleep(500);

        if (strcmp(buffer, "PING") == 0)
        {
            pongInvoker(fifo);
        }
        else if (strcmp(buffer, "KILL") == 0)
        {
            deamonSay("Stoping the deamon");
            deamonIsListening = 0;
        }
        else if (strcmp(buffer, "TIME") == 0)
        {
            time_t currentTime = time(0);
            char response[BUFFER_SIZE];
            sprintf(response, "%ld", currentTime);
            send(fifo, response);
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
    deamonSay("Hello !");

    // Get fifo name from the fist argument
    t_fifo myFifo = argv[1];

    deamonSay("Using fifo [%s]", myFifo);

    int returnCode = listening(myFifo);

    deamonSay("Good bye !");
    return returnCode;
}
