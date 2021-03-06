#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * \brief Retuourne la taille de la chaine donnée en paramètre
 */
int strLength(char *str)
{
    int cpt;
    for (cpt = 0; str[cpt] != '\0'; cpt++)
        ;
    return cpt;
}

/**
 * \brief Compare 2 chaines de caractères, retourne 1 si elles sont identiques sinon 0.
 * \param chaine1 Première chaine
 * \param chaine2 Seconde chaine
 */
int areEquals(char *chaine1, char *chaine2)
{
    int i;
    for (i = 0; chaine1[i] != '\0'; i++)
    {
        if (chaine1[i] != chaine2[i])
        {
            return 0;
        }
    }
    return chaine2[i] == '\0';
}

/**
 * \brief 
 */
int listenWithTimeout(char *filename, char *message)
{
    pid_t listener, timer = -1;

    int tube[2];
    if (pipe(tube) != 0)
    {
        perror("Error creating the pipe");
        return 1;
    }

    // Fork children
    listener = fork();
    if (listener == -1)
        return 1;
    else if (listener == 0)
    {
        // Processus fils : écouteur
        close(tube[0]);
        listen(filename, message);
        write(tube[1], message, BUFFER_SIZE);
        exit(0);
    }
    else
    {
        timer = fork();
        if (timer == -1)
            return 1;

        else if (timer == 0)
        {
            // Processus fils Timer
            sleep(CONNECTION_TIMEOUT);
            exit(0);
        }
        else
        {
            // Wait for the first child. The wait function return the pid of the first son that ended its process
            pid_t firstChild = wait(NULL);

            if (firstChild == timer)
            {
                fprintf(stderr, "Connection timeout\n");
                kill(listener, KILL_SIGNAL);
                return 2;
            }

            close(tube[1]);
            read(tube[0], message, BUFFER_SIZE);
            kill(timer, KILL_SIGNAL);

            return 0;
        }
    }
}
