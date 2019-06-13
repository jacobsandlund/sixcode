#include "gpu/renderer.h"
#include <stdlib.h>

struct GpuRenderer {
    float2 viewport_size;
    GpuDevice *device;
    GpuView *drew_in_view;
};

GpuRenderer *gpu_renderer_create(GpuDevice *device, GpuView *view, float2 viewport_size)
{
    (void) view;
    GpuRenderer *renderer = malloc(sizeof *renderer);
    renderer->device = device;
    renderer->drew_in_view = NULL;
    renderer->viewport_size = viewport_size;
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

void gpu_renderer_size_changed(GpuRenderer *renderer, GpuView *view, float2 viewport_size)
{
    (void) view;
    renderer->viewport_size = viewport_size;
}
