#ifndef __USER_STRING_H
#define __USER_STRING_H
#include <stdlib.h>
 static __inline size_t strnlen(const char *s, size_t count)
{
        const char *sc;

        for (sc = s; count-- && *sc != '\0'; ++sc)
                /* nothing */;  
        return sc - s;  
}

#endif //__USER_STRING_H
