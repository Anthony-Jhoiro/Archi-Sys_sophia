#ifndef INVOKER_H
#define INVOKER_H

#include "../tools/tools.h"

/**
 * \return 1 if deamon is active
 */
int pingDeamon(t_fifo fifo);

/**
 * 
 */
int createDeamon(t_fifo fifo);

#endif