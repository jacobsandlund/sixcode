#include "gpu/renderer.h"
#include <stdlib.h>

struct GpuRenderer {
    GpuDevice *device;
    GpuView *drew_in_view;
};

GpuRenderer *gpu_renderer_create(GpuDevice *device, GpuView *view)
{
    (void) view;
    GpuRenderer *renderer = malloc(sizeof *renderer);
    renderer->device = device;
    renderer->drew_in_view = NULL;
    return renderer;
}

void gpu_renderer_destroy(GpuRenderer *renderer)
{
    free(renderer);
}

void gpu_renderer_draw_in_view(GpuRenderer *renderer, GpuView *view)
{
    renderer->drew_in_view = view;
}
