#include "deamon.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/**
 * \brief Ecoute et réagit aux événements
 * \return 0 si la fonction a été executée sans problème
 */
int listening(t_fifo fifo)
{
    char buffer[BUFFER_SIZE];

    time_t baseTime = time(0);

    int deamonIsListening = 1;
    while (deamonIsListening)
    {
        int fd = open(fifo, O_RDONLY);
        FILE *fp = fdopen(fd, "r");
        fscanf(fp, "%s", buffer); // We are using fscanf here to pause the process unitil the next input
        close(fd);

        if (strlen(buffer) == 0)
            continue;

        deamonSay("recieved [%s]", buffer);
        // usleep(200);

        if (strcmp(buffer, PING_MESSAGE) == 0)
        {
            // Send a PONG to the invoker
            pongInvoker(fifo);
        }
        else if (strcmp(buffer, KILL_MESSAGE) == 0)
        {
            // Stop listening for events => Stop the deamon
            killDeamon(&deamonIsListening);
        }
        else if (strcmp(buffer, DATE_MESSAGE) == 0)
        {
            // Send the current date
            sendDate(fifo);
        }
        else if (strcmp(buffer, RESET_MESSAGE) == 0)
        {
            // Reset the base time
            resetDeamon(&baseTime);
        }
        else if (strcmp(buffer, DURATION_MESSAGE) == 0)
        {
            // Send the difference between the current time and the base time
            sendDuration(fifo, baseTime);
        }
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
    deamonSay("Bonjour !");

    // Get fifo name from the fist argument
    t_fifo myFifo = argv[1];

    int returnCode = listening(myFifo);

    deamonSay("A+ !");
    return returnCode;
}

// ---------------- deamon's functions

/**
 * \brief Envoie "PONG" à l'invocateur
 */
void pongInvoker(t_fifo fifo)
{
    send(fifo, PONG_MESSAGE);
}

/**
 *  \brief envoie la date à l'invoker
 *  
 *  \param t_fifo : fifo qui communique avec l'invoker
 */
void sendDate(t_fifo fifo)
{
    time_t currentTime = time(0);
    char response[BUFFER_SIZE];
    sprintf(response, "%ld", currentTime);
    send(fifo, response);
}

/**
 *  \brief envoie la durée de vie de sophia à l'invoker
 *  
 *  \param t_fifo : fifo qui communique avec l'invoker
 */
void sendDuration(t_fifo fifo, time_t baseTime)
{
    time_t timeDelta = time(0) - baseTime;
    char response[BUFFER_SIZE];
    sprintf(response, "%ld", timeDelta);
    send(fifo, response);
}

/**
 *  \brief modifie l'état d'écoute de sophia à 0 (= n'écoute pas)
 *  
 *  \param int* : entier qui indique l'état d'écoute du deamon
 */
void killDeamon(int *deamonIsListening)
{
    *deamonIsListening = 0;
}

/**
 *  \brief modifie la date d'appel de sophia à la date actuelle
 *  
 *  \param time_t* : ancienne date d'appel à modifier
 */
void resetDeamon(time_t *baseTime)
{
    *baseTime = time(0);
}
