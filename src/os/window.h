#ifndef OsWindow_h
#define OsWindow_h

#include "spacetime.h"
#include "gpu/view.h"

typedef struct OsWindow OsWindow;

OsWindow *os_window_create(GpuView *view);
void os_window_destroy(OsWindow *window);
void os_window_show(OsWindow *window);

#endif // OsWindow_h
