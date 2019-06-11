#ifndef OsScreen_h
#define OsScreen_h

#include "spacetime.h"

typedef struct {
    float2 origin;
    float2 size;
} OsScreenFrame;

OsScreenFrame os_screen_visible_frame(void);

#endif // OsScreen_h
