#ifndef CONSTANTS_H
#define CONSTANTS_H

#define BUFFER_SIZE 256

#define FIFO_MODE 0666

/**
 * Time to wait a response of a process (in seconds)
 */
#define CONNECTION_TIMEOUT 4

#define FIFO_NAME "/tmp/sophia"

#define KILL_SIGNAL 9

// --- Messages --- //
#define PING_MESSAGE "PING"
#define PONG_MESSAGE "PONG"
#define KILL_MESSAGE "KILL"
#define DATE_MESSAGE "TIME"
#define DURATION_MESSAGE "DURATION"
#define RESET_MESSAGE "RESET"
#define PONG_MESSAGE "PONG"
#define HELP_FILE_NAME "help.txt"

// --- Commandes --- //
#define START_COMMANDE "--start"
#define STATE_COMMANDE "--state"
#define HELP_COMMANDE "--help"
#define DATE_COMMANDE "--date"
#define DURATION_COMMANDE "--duration"
#define RESET_COMMANDE "--reset"
#define STOP_COMMANDE "--stop"
#define RESTART_COMMANDE "--restart"

#endif