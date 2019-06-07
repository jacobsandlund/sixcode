#ifndef OsWindow_h
#define OsWindow_h

#include "spacetime.h"
#include "gpu/view.h"

typedef struct {
    void *window_impl;
    GpuView *view;
} OsWindow;

void os_window_init(OsWindow *window);
void os_window_destroy(OsWindow *window);
void os_window_set_view(OsWindow *window, GpuView *view);
float2 os_window_size(OsWindow *window);
void os_window_show(OsWindow *window);

#endif // OsWindow_h
