#ifndef __USER_STDIO_H
#define __USER_STDIO_H
#include "stm32f4xx.h"
#include <stdio.h>
#define DEBUG_LOG(...) printf("Filename %s, Function %s, Line %d > ", __FILE__, __FUNCTION__, __LINE__); \
                            printf(__VA_ARGS__); \
                            printf("\n");
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#endif //#ifndef __USER_STDIO_H
