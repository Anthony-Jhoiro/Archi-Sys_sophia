#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int strLength(char *str)
{
    int cpt;
    for (cpt = 0; str[cpt] != '\0'; cpt++)
        ;
    return cpt;
}

int isFifoOpen(t_fifo fifo)
{
    struct stat st;
    stat(fifo, &st);
    return S_ISFIFO(st.st_mode);
}

/**
 * \brief Open a connection between invoker and deamon
 * \param filename The filename for the communication
 * \return 0 if the pipe oppened successfully
 */
int getConnection(char *filename)
{
    int fd = open(filename, O_RDONLY);
    FILE *fp = fdopen(fd, "r");
}

int createConnection(char *filename)
{
    unlink(filename);
    if (mkfifo(filename, 0644) != 0)
    {
        perror("Fail to create Fifo");
        return 3;
    }
    return 0;
}

int closeConnection(char *filename)
{
    return unlink(filename);
}

int send(t_fifo fifo, char *message)
{
    int fd = open(fifo, O_WRONLY);
    if (fd == -1)
        return 1;

    char buffer[BUFFER_SIZE];

    // TODO : is buffer necessary ?
    sprintf(buffer, "%s\n", message);

    // FILE *fp = fdopen(fd, "w");
    // fprintf(fp, "%s", buffer);

    write(fd, buffer, strLength(buffer));

    close(fd);
    return 0;
}

void listen(t_fifo fifo, char *buffer)
{
    int fd = open(fifo, O_RDONLY);

    read(fd, buffer, BUFFER_SIZE);

    close(fd);
}

/**
 * \return 0 si ça marche
 */
int listenWithTimeout(char *filename, char *message)
{
    pid_t listener, timer = -1;
    printf("Listen with timeout\n");

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
    else if (listener == 0 && timer != 0) // TODO : is timer != 0 necessary ?
    {
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

            sleep(CONNECTION_TIMEOUT);
            exit(0);
        }
        else
        {
            // Wait for the first child. The wait function return the pid of the first son that ended its process
            pid_t firstChild = wait(NULL);
            printf("%d, %d, %d\n", listener, timer, firstChild);

            if (firstChild == timer)
            {
                fprintf(stderr, "Connection timeout\n");
                // TODO : Replace 9 with SIGKILL
                kill(listener, 9);
                return 2;
            }

            close(tube[1]);
            read(tube[0], message, BUFFER_SIZE);
            // TODO : Replace 9 with SIGKILL
            kill(timer, 9);

            return 0;
        }
    }
}
