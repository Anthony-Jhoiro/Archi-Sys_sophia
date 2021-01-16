#ifndef TOOLS_H
#define TOOLS_H

#include "constants.h"
// FIFO

typedef char *t_fifo;

int forceFifoCreation(t_fifo fifo);

int destroyFifo(t_fifo fifo);

int send(t_fifo fifo, char *message);

void listen(t_fifo fifo, char *message);

int isFifoOpen(t_fifo fifo);

// Utilitaires
int listenWithTimeout(char *filename, char *message);

int strLength(char *str);

int areEquals(char *chaine1, char *chaine2);

void invokerSay(char *format, ...);

void deamonSay(char *format, ...);

#endif