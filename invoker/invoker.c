#include "invoker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

/**
 * \brief Crée un démon. Le démon est créé dans un nouveau processus et la fifo est créée de manière forte. 
 * Si la fifo existait déjà, la détruit et en recrée une.
 * \return 0 si le démon a été créé sans problème
 */
int createDeamon(t_fifo fifo)
{
    pid_t deamonProcess = fork();
    if (deamonProcess == -1)
    {
        perror("Impossible d'invoquer Sophia");
        return 1;
    }
    else if (deamonProcess == 0)
    {
        // son process
        execlp("../deamon/deamon", "deamon", fifo, NULL);
    }
    return forceFifoCreation(fifo);
}

/**
 * \brief Envoie un PING au démon via la fifo donnée en paramètre.
 * \param fifo Fifo utilisée pour communiquer avec le démon. Elle doit être initialisée précédement.
 * \return 0 si l'envoie a fonctionné
 */
int pingDeamon(t_fifo fifo)
{
    return send(fifo, PING_MESSAGE);
}

/**
 * \brief Ping le démon pour savoir si il est en vie.
 * \return 0 si le démon est en vie
 */
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

    invokerSay("Recieved [%s]", buffer);
    if (!areEquals(PONG_MESSAGE, buffer))
        return 1;

    return 0;
}

/**
 * \brief Arrête le démon et supprime la fifo
 * \return 0 si le message de destruction a bien pu être envoyer
 */
int killDeamon(t_fifo fifo)
{
    if (!isFifoOpen(fifo))
    {
        return -1;
    }
    // Send 'KILL' to the deamon
    int sendKill = send(fifo, KILL_MESSAGE);

    // Destrou the fifo
    destroyFifo(fifo);
    return sendKill;
}

/**
 * \brief Affiche l'état du démon 
 * \return 0 si le démon est en vie; 1 sinon.
 */
int cmdState(t_fifo fifo)
{
    if (isDeamonAlive(fifo) == 0)
    {
        invokerSay("Sophia est en vie");
        return 0;
    }
    invokerSay("Sophia est injoignable");
    return 1;
}

/**
 * \brief Crée un démon
 * \return 0 si ça s'est bien passé
 */
int cmdStart(t_fifo fifo)
{
    if (createDeamon(fifo) == 0)
    {
        invokerSay("Sophia a été invoquée");
        return 0;
    }
    invokerSay("Sophia n'a pas pu être invoquée");
    return 1;
}

/**
 * \brief Arrête le démon
 * \return 0 si ça c'est bien passé 
 */
int cmdStop(t_fifo fifo)
{
    if (killDeamon(fifo) == 0)
    {
        invokerSay("Sophia a été renoyée en enfer");
        return 0;
    }
    invokerSay("Impossible de renvoyer Sophia en enfer.");
    return 1;
}

/**
 * \brief Redémarre le démon
 * \return 0 si ça c'est bien passé 
 */
int cmdRestart(t_fifo fifo)
{
    invokerSay("Ré-invocation de Sophia");
    cmdStop(fifo);
    return cmdStart(fifo);
}

/**
 * \brief Récupère la date par le démon
 * \return 0 si ça c'est bien passé 
 */
int cmdDate(t_fifo fifo)
{
    if (!isFifoOpen(fifo))
    {
        return -1;
    }
    int sendSuccess = send(fifo, DATE_MESSAGE);
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

/**
 * \brief Récupère la durée de vie du démon
 * \return 0 si ça c'est bien passé 
 */
int cmdDuration(t_fifo fifo)
{
    if (!isFifoOpen(fifo))
    {
        return -1;
    }
    int sendSuccess = send(fifo, DURATION_MESSAGE);
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

/**
 * \brief Réinitialise la date de création du démon
 * \return 0 si ça c'est bien passé 
 */
int cmdReset(t_fifo fifo)
{
    if (!isFifoOpen(fifo))
    {
        return -1;
    }
    return send(fifo, "RESET");
}

/**
 * \brief Affiche l'aide depuis le fichier help.txt
 * \return 0 si ça c'est bien passé 
 */
int cmdHelp()
{
    FILE *file = fopen(HELP_FILE_NAME, "r");
    char c;

    while ((c = fgetc(file)) != EOF)
    {
        printf("%c", c);
    }
    fclose(file);

    return 0;
}

int main(int argc, char *argv[])
{
    t_fifo myFifo = FIFO_NAME;

    if (argc == 1)
    {
        invokerSay("Au moins un argument est nécéssaire");
        return 1;
    }

    char *command = argv[1];

    if (areEquals(command, START_COMMANDE))
    {
        return cmdStart(myFifo);
    }
    else if (areEquals(command, STATE_COMMANDE))
    {
        return cmdState(myFifo);
    }
    else if (areEquals(command, HELP_COMMANDE))
    {
        return cmdHelp();
    }
    else if (areEquals(command, DATE_COMMANDE))
    {
        return cmdDate(myFifo);
    }
    else if (areEquals(command, DURATION_COMMANDE))
    {
        return cmdDuration(myFifo);
    }
    else if (areEquals(command, RESET_COMMANDE))
    {
        return cmdReset(myFifo);
    }
    else if (areEquals(command, STOP_COMMANDE))
    {
        return cmdStop(myFifo);
    }
    else if (areEquals(command, RESTART_COMMANDE))
    {
        return cmdRestart(myFifo);
    }
    else
    {
        fprintf(stderr, "Commande inconnue [%s]\n", command);
    }

    return 0;
}
