#ifndef _OsScreen_h
#define _OsScreen_h

#include "Spacetime.h"

typedef struct {
    float2 origin;
    float2 size;
} OsScreenFrame;

OsScreenFrame OsScreenVisibleFrame(void);

#endif // _OsScreen_h
