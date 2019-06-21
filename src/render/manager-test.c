#include "render/manager.c"
#include "gpu/view-mock.c"
#include "gpu/device-mock.c"
#include "render/layout.c"
#include "test.h"

Test(RenderManagerInit)
{
    RenderManagerConfig config = {
        .layout_type = RenderLayoutTypeRect,
        .viewport_size = {2560, 1440},
    };

    RenderManagerInit(&config);

    _d(gRenderManager.viewport.layout.type == RenderLayoutTypeRect);
    //=> 1
    _f2(gRenderManager.viewport.size);
    //=> 2560, 1440

    RenderManagerDestroy();
}

Test(RenderManagerSizeChanged)
{
    float2 old_viewport_size = {1920, 1080};
    float2 new_viewport_size = {2560, 1920};
    RenderManagerConfig render_config = {
        .layout_type = RenderLayoutTypeRect,
    };

    OsScreenFrame visible_frame = {
        .origin = {0, 0},
        .size = old_viewport_size,
    };
    GpuDevice *device = GpuDeviceCreate();
    GpuView *view = GpuViewCreate(device, visible_frame, &gGpuViewMockConfig);

    render_config.viewport_size = GpuViewViewportSize(view);

    RenderManagerInit(&render_config);

    _f2(gRenderManager.viewport.size);
    //=> 1920, 1080

    GpuViewCallbacks view_callbacks = {
        .size_changed = RenderManagerSizeChanged,
    };

    GpuViewRegisterCallbacks(view, &view_callbacks);
    gpu_view_mock_size_changed(view, new_viewport_size);

    _f2(gRenderManager.viewport.size);
    //=> 2560, 1920

    RenderManagerDestroy();
    GpuViewDestroy(view);
    GpuDeviceDestroy(device);
}
