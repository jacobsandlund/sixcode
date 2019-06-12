#include "gpu/view.h"
#include "test.h"
#include <stdlib.h>

struct {
    GpuDevice *device;
    float2 viewport_size;
    GpuViewConfig config;
} GpuView;

void gpu_view_mock_size_changed(GpuView *view, float2 viewport_size)
{
    view->viewport_size = viewport_size;
    view->config.size_changed(view, viewport_size);
}

GpuView *gpu_view_create(GpuDevice *device, float2 viewport_size, GpuViewConfig *config)
{
    GpuView *view = tmalloc(sizeof *view);
    view->device = device;
    view->config = *config;
    gpu_view_mock_size_changed(view, viewport_size);

    return view;
}

void gpu_view_destroy(GpuView *view)
{
    tfree(view);
}

float2 gpu_view_viewport_size(GpuView *view)
{
    return view->viewport_size;
}
