#include "gpu/view.h"
#include "test-allocator.h"

struct GpuView {
    GpuDevice *device;
    float2 viewport_size;
    GpuViewFn draw_in_view;
    GpuViewSizeChangedFn size_changed;
};

void gpu_view_mock_size_changed(GpuView *view, float2 viewport_size)
{
    view->viewport_size = viewport_size;
    view->size_changed(view, viewport_size);
}

void gpu_view_mock_draw_in_view(GpuView *view)
{
    view->draw_in_view(view);
}

GpuView *gpu_view_create(GpuDevice *device, OsScreenFrame frame, GpuViewConfig *config)
{
    GpuView *view = tmalloc(sizeof *view);
    view->device = device;
    view->draw_in_view = config->draw_in_view;
    view->size_changed = config->size_changed;
    gpu_view_mock_size_changed(view, frame.size);

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
