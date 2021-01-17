#include "tools.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
/**
 * \brief Cette fonction envoie un message dans la fifo
 * 
 * \param t_fifo : fifo pour écrire le message
 * \param char* : message à envoyer
 * 
 * \return int : 0 si réussite, 1 si impossible d'ouvrir la fifo
 */
int send(t_fifo fifo, char *message)
{
    int fd = open(fifo, O_WRONLY);
    if (fd == -1)
        return 1;

    char buffer[BUFFER_SIZE];

    sprintf(buffer, "%s\n", message);

    // En C la tail d'un char est un octet, on peut donc renvoyer directement
    // la taille de la chaine pour le nombre d'octet de celle ci
    write(fd, message, strLength(buffer));

    close(fd);
    return 0;
}

/**
 * \brief Cette fonction lit une fifo pour en tirer un message
 * 
 * \param t_fifo : fifo pour lire le message
 * \param char* : buffer où enregistrer
 * 
 * \return void
 */
void listen(t_fifo fifo, char *buffer)
{
    int fd = open(fifo, O_RDONLY);

    read(fd, buffer, BUFFER_SIZE);

    close(fd);
}

/**
 * \brief Cette fonction détruit une fifo
 * 
 * \param t_fifo : fifo à détruire
 * 
 * \return int : résultat de la fonction unlink
 */
int destroyFifo(t_fifo fifo)
{
    return unlink(fifo);
}

/**
 * \brief Cette fonction force la création d'une nouvelle fifo, même s'il en existe déjà une
 * 
 * \param t_fifo : fifo à créer ou recréer
 * 
 * \return int : 0 si la fifo a pu être créée (ou recréée), 1 s'il y a un problème dans la construction de la fifo
 */
int forceFifoCreation(t_fifo fifo)
{
    if (mkfifo(fifo, FIFO_MODE) != 0)
    {
        unlink(fifo);
        if (mkfifo(fifo, FIFO_MODE) != 0)
        {
            perror("[ERROR] fifo creation");
            return 1;
        }
    }
    return 0;
}

/**
 * \brief Cette fonction retourne le statut d'une fifo
 * 
 * \param t_fifo : fifo à tester
 * 
 * \return int : 0 si la fifo est fermée, 1 si ouverte
 */
int isFifoOpen(t_fifo fifo)
{
    struct stat st;
    stat(fifo, &st);
    return S_ISFIFO(st.st_mode);
}