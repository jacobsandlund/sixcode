#include "Gpu/View.h"

#include <stdlib.h>

struct GpuView {
    GpuDevice *device;
    i64 preferred_frames_per_second;
    i64 color_pixel_format;
    float2 viewport_size;
    GpuViewFn draw_in_view;
    GpuViewSizeChangedFn size_changed;
    bool has_current_render_pass;
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

static void gpu_view_mock_noop_draw_in_view(GpuView *view)
{
    (void)view;
}

static void gpu_view_mock_noop_size_changed(GpuView *view, float2 viewport_size)
{
    (void)view;
    (void)viewport_size;
}

GpuViewCallbacks gGpuViewMockNoopCallbacks = {
    .draw_in_view = gpu_view_mock_noop_draw_in_view,
    .size_changed = gpu_view_mock_noop_size_changed,
};

GpuViewConfig gGpuViewMockConfig = {
    .preferred_frames_per_second = 60,
    .color_pixel_format = 1,
};

GpuView *GpuViewCreate(GpuDevice *device,
                       OsScreenFrame frame,
                       GpuViewConfig *config)
{
    GpuView *view = malloc(sizeof *view);

    view->device = device;
    view->preferred_frames_per_second = config->preferred_frames_per_second;
    view->color_pixel_format = config->color_pixel_format;
    view->has_current_render_pass = true;

    GpuViewRegisterCallbacks(view, &gGpuViewMockNoopCallbacks);
    gpu_view_mock_size_changed(view, frame.size);

    return view;
}

void GpuViewDestroy(GpuView *view)
{
    free(view);
}

void GpuViewRegisterCallbacks(GpuView *view, GpuViewCallbacks *callbacks)
{
    view->draw_in_view = callbacks->draw_in_view;
    view->size_changed = callbacks->size_changed;
}

float2 GpuViewViewportSize(GpuView *view)
{
    return view->viewport_size;
}

GpuRenderPassConfig *GpuViewCurrentRenderPassConfig(GpuView *view)
{
    if (view->has_current_render_pass) {
        // Return an arbitrary pointer
        return (GpuRenderPassConfig *)view;
    } else {
        return NULL;
    }
}
