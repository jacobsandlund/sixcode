#ifndef OsWindow_h
#define OsWindow_h

#include "spacetime.h"
#include "gpu/view.h"

typedef struct {
    void *window_impl;
    GpuView *view;
} OsWindow;

void os_window_init(OsWindow *window, GpuView *view);
void os_window_destroy(OsWindow *window);
void os_window_show(OsWindow *window);

#endif // OsWindow_h
