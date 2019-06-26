#ifndef _OsWindow_h
#define _OsWindow_h

#include "Gpu/View.h"
#include "Spacetime.h"

typedef struct OsWindow OsWindow;

OsWindow *OsWindowCreate(GpuView *view);
void OsWindowDestroy(OsWindow *window);
void OsWindowShow(OsWindow *window);

#endif  // _OsWindow_h
