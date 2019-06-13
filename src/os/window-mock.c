#include "os/window.h"
#include "test/allocator.h"

struct OsWindow {
    GpuView *view;
    bool showing;
};

OsWindow *os_window_create(GpuView *view)
{
    OsWindow *window = tmalloc(sizeof *window);
    window->view = view;
    window->showing = false;

    return window;
}

void os_window_destroy(OsWindow *window)
{
    tfree(window);
}

void os_window_show(OsWindow *window)
{
    window->showing = true;
}
