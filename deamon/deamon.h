#ifndef DEAMON_H
#define DEAMON_H

#include <time.h>
#include "../tools/tools.h"

/**
 * \brief Envoie "PONG" à l'invocateur
 */
void pongInvoker(t_fifo fifo);

/**
 *  \brief envoie la date à l'invoker
 *  
 *  \param t_fifo : fifo qui communique avec l'invoker
 */
void sendDate(t_fifo fifo);

/**
 *  \brief envoie la durée de vie de sophia à l'invoker
 *  
 *  \param t_fifo : fifo qui communique avec l'invoker
 */
void sendDuration(t_fifo fifo, time_t baseTime);

/**
 *  \brief modifie l'état d'écoute de sophia à 0 (= n'écoute pas)
 *  
 *  \param int* : entier qui indique l'état d'écoute du deamon
 */
void killDeamon(int* deamonIsListening);

/**
 *  \brief modifie la date d'appel de sophia à la date actuelle
 *  
 *  \param time_t* : ancienne date d'appel à modifier
 */
void resetDeamon(time_t* baseTime);

// void *listenForEvent(connection conn);

#endif