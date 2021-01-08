#ifndef TOOLS_H
#define TOOLS_H

#define BUFFER_SIZE 256

typedef struct m_connection
{
    char *filename;
    int tubeDeamon[2];
    int tubeInvoker[2];
} connection;
typedef

    /**
 * \brief Open a connection between invoker and deamon
 * \param filename The filename for the communication
 * \return 0 if the pipe oppened successfully
 */
    int
    openConnection(char *filename);

/**
 * \return 1 si ça marche
 */
int send(int *tube, char *message);

/**
 * \return 1 si ça marche
 */
int listen(int *tube, char *message);

#endif