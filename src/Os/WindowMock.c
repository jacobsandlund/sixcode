#include "Os/Window.h"

#include <stdlib.h>

struct OsWindow {
    GpuView *view;
    bool showing;
};

OsWindow *OsWindowCreate(GpuView *view)
{
    OsWindow *window = malloc(sizeof *window);
    window->view = view;
    window->showing = false;

    return window;
}

void OsWindowDestroy(OsWindow *window)
{
    free(window);
}

void OsWindowShow(OsWindow *window)
{
    window->showing = true;
}
