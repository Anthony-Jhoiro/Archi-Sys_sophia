#ifndef INVOKER_H
#define INVOKER_H

#include "../tools/tools.h"

// TODO : check rights
#define FIFO_MODE 0666

/**
 * \return 1 if deamon is active
 */
int pingDeamon(t_fifo fifo);

/**
 * 
 */
int createDeamon(t_fifo fifo);

#endif