#ifndef _OsScreen_h
#define _OsScreen_h

#include "spacetime.h"

typedef struct {
    float2 origin;
    float2 size;
} OsScreenFrame;

OsScreenFrame OsScreenVisibleFrame(void);

#endif // _OsScreen_h
