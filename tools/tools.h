#ifndef TOOLS_H
#define TOOLS_H

#define BUFFER_SIZE 256
typedef char *t_fifo;

/**
 * Time towait a response of a process
 */
#define CONNECTION_TIMEOUT 10

/**
 * \brief Open a connection between invoker and deamon
 * \param filename The filename for the communication
 * \return 0 if the pipe oppened successfully
 */
int openConnection(char *filename);

void send(t_fifo fifo, char *message);

void listen(t_fifo fifo, char *message);

int listenWithTimeout(char *filename, char *message);

#endif