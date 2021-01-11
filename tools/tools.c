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

void send(t_fifo fifo, char *message)
{
    int fd = open(fifo, O_WRONLY);

    char buffer[BUFFER_SIZE];

    // TODO : is buffer necessary ?
    sprintf(buffer, "%s\n", message);

    // FILE *fp = fdopen(fd, "w");
    // fprintf(fp, "%s", buffer);

    write(fd, buffer, strLength(buffer));

    close(fd);
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
    pid_t listener, timer;

    // Fork children
    listener = fork();
    printf("listener = %d\n", listener);
    if (listener == -1)
        return 1;
    else if (listener == 0)
    {
        printf("Listen for input...\n");
        listen(filename, message);
    }
    else
    {
        printf("Timer\n");

        timer = fork();
        if (timer == -1)
            return 1;

        // listener

        // timer
        else if (timer == 0)
        {
            sleep(CONNECTION_TIMEOUT);
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
            // TODO : Replace 9 with SIGKILL
            kill(timer, 9);

            return 0;
        }
    }
}

// int main(int argc, char const *argv[])
// {
//     char buffer[256];
//     char *filename = "/tmp/sophia";
//     createConnection(filename);

//     printf("Send hello \n");
//     send(filename, "Hello !");

//     printf("Start listening\n");
//     int ret = listen(filename, buffer);

//     printf("[%d] => %s\n", ret, buffer);

//     closeConnection(filename);

//     return 0;
// }
