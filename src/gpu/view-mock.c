#include "gpu/view.h"
#include <stdlib.h>

typedef struct {
    GpuDevice *device;
    float2 viewport_size;
    GpuViewConfig config;
} GpuViewMock;

void gpu_view_mock_size_changed(GpuView *view, float2 viewport_size)
{
    GpuViewMock *mock = (GpuViewMock *)view->view_impl;
    view->viewport_size = viewport_size;
    mock->config.size_changed(view);
}

void gpu_view_init(GpuView *view, GpuDevice *device, float2 viewport_size, GpuViewConfig *config)
{
    GpuViewMock *mock = malloc(sizeof *mock);
    view->view_impl = (void *) mock;
    mock->device = device;
    mock->config = *config;
    gpu_view_mock_size_changed(view, viewport_size);
}

void gpu_view_destroy(GpuView *view)
{
    free(view->view_impl);
}
