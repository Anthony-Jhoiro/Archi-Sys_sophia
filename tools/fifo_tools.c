#include "tools.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

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

void listen(t_fifo fifo, char *buffer)
{
    int fd = open(fifo, O_RDONLY);

    read(fd, buffer, BUFFER_SIZE);

    close(fd);
}

int destroyFifo(t_fifo fifo)
{
    return unlink(fifo);
}

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

int isFifoOpen(t_fifo fifo)
{
    struct stat st;
    stat(fifo, &st);
    return S_ISFIFO(st.st_mode);
}