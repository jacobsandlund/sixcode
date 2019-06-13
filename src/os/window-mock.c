#include "os/window.h"
#include <stdlib.h>

struct OsWindow {
    GpuView *view;
    bool showing;
};

OsWindow *os_window_create(GpuView *view)
{
    OsWindow *window = malloc(sizeof *window);
    window->view = view;
    window->showing = false;

    return window;
}

void os_window_destroy(OsWindow *window)
{
    free(window);
}

void os_window_show(OsWindow *window)
{
    window->showing = true;
}
