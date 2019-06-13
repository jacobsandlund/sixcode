#include "gpu/renderer.c"
#include "gpu/device-mock.c"
#include "gpu/view-mock.c"
#include "test.h"

GpuRenderer *renderer;

void test_gpu_renderer_draw_in_view(GpuView *view)
{
    gpu_renderer_draw_in_view(renderer, view);
}

void test_gpu_renderer_size_changed(GpuView *view, float2 viewport_size)
{
    gpu_renderer_size_changed(renderer, view, viewport_size);
}

GpuViewCallbacks test_callbacks = {
    .draw_in_view = test_gpu_renderer_draw_in_view,
    .size_changed = test_gpu_renderer_size_changed,
};

Test(gpu_renderer)
{
    OsScreenFrame frame = {
        .origin = {0, 0},
        .size = {640, 480},
    };

    GpuDevice *device = gpu_device_create();
    GpuView *view = gpu_view_create(device, frame, &gGpuViewMockConfig);

    float2 viewport_size = gpu_view_viewport_size(view);
    renderer = gpu_renderer_create(device, view, viewport_size);

    gpu_view_register_callbacks(view, &test_callbacks);

    gpu_renderer_destroy(renderer);

    gpu_view_destroy(view);
    gpu_device_destroy(device);
}
