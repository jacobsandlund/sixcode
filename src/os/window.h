#ifndef _OsWindow_h
#define _OsWindow_h

#include "spacetime.h"
#include "gpu/view.h"

typedef struct OsWindow OsWindow;

OsWindow *OsWindowCreate(GpuView *view);
void OsWindowDestroy(OsWindow *window);
void OsWindowShow(OsWindow *window);

#endif // _OsWindow_h
