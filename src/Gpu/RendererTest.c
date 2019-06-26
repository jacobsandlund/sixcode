#include "Gpu/Renderer.c"

#include "Gpu/DeviceMock.c"
#include "Gpu/ViewMock.c"

#include "Test.h"

GpuRenderer *renderer;

void test_gpu_renderer_draw_in_view(GpuView *view)
{
    GpuRendererDrawInView(renderer, view);
}

void test_gpu_renderer_size_changed(GpuView *view, float2 viewport_size)
{
    GpuRendererSizeChanged(renderer, view, viewport_size);
}

GpuViewCallbacks test_callbacks = {
    .draw_in_view = test_gpu_renderer_draw_in_view,
    .size_changed = test_gpu_renderer_size_changed,
};

Test(gpu_renderer)
{
    OsScreenFrame frame = {
        .origin = { 0, 0 },
        .size = { 640, 480 },
    };

    GpuDevice *device = GpuDeviceCreate();
    GpuView *view = GpuViewCreate(device, frame, &gGpuViewMockConfig);

    float2 viewport_size = GpuViewViewportSize(view);
    renderer = gpu_renderer_create(device, view, viewport_size);

    GpuViewRegisterCallbacks(view, &test_callbacks);

    GpuRendererDestroy(renderer);

    GpuViewDestroy(view);
    GpuDeviceDestroy(device);
}
