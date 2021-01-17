#ifndef INVOKER_H
#define INVOKER_H

#include "../tools/tools.h"

/**
 * \brief Crée un démon. Le démon est créé dans un nouveau processus et la fifo est créée de manière forte. 
 * Si la fifo existait déjà, la détruit et en recrée une.
 * \return 0 si le démon a été créé sans problème
 */
int createDeamon(t_fifo fifo);

/**
 * \brief Envoie un PING au démon via la fifo donnée en paramètre.
 * \param fifo Fifo utilisée pour communiquer avec le démon. Elle doit être initialisée précédement.
 * \return 0 si l'envoie a fonctionné
 */
int pingDeamon(t_fifo fifo);

/**
 * \brief Ping le démon pour savoir si il est en vie.
 * \return 0 si le démon est en vie
 */
int isDeamonAlive(t_fifo fifo);

/**
 * \brief Arrête le démon et supprime la fifo
 * \return 0 si le message de destruction a bien pu être envoyer
 */
int killDeamon(t_fifo fifo);

/**
 * \brief Affiche l'état du démon 
 * \return 0 si le démon est en vie; 1 sinon.
 */
int cmdState(t_fifo fifo);

/**
 * \brief Crée un démon
 * \return 0 si ça s'est bien passé
 */
int cmdStart(t_fifo fifo);

/**
 * \brief Arrête le démon
 * \return 0 si ça c'est bien passé 
 */
int cmdStop(t_fifo fifo);

/**
 * \brief Redémarre le démon
 * \return 0 si ça c'est bien passé 
 */
int cmdRestart(t_fifo fifo);

/**
 * \brief Récupère la date par le démon
 * \return 0 si ça c'est bien passé 
 */
int cmdDate(t_fifo fifo);

/**
 * \brief Récupère la durée de vie du démon
 * \return 0 si ça c'est bien passé 
 */
int cmdDuration(t_fifo fifo);

/**
 * \brief Réinitialise la date de création du démon
 * \return 0 si ça c'est bien passé 
 */
int cmdReset(t_fifo fifo);

/**
 * \brief Affiche l'aide depuis le fichier help.txt
 * \return 0 si ça c'est bien passé 
 */
int cmdHelp();

#endif