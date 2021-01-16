#include "tools.h"
#include <stdio.h>
#include <stdarg.h>

void invokerSay(char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("\e[0;33m[INVOKER] ");

    vprintf(format, args);

    printf("\e[0m\n");

    va_end(args);
}

void deamonSay(char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("\e[0;35m[DEAMON] ");

    vprintf(format, args);

    printf("\e[0m\n");

    va_end(args);
}