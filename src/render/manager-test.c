#include "render/manager.c"
#include "gpu/view-mock.c"
#include "gpu/device-mock.c"
#include "render/layout.c"
#include "test.h"

Test(render_manager_init)
{
    RenderManagerConfig config = {
        .layout_type = RenderLayoutTypeRect,
        .viewport_size = {2560, 1440},
    };

    render_manager_init(&config);

    _d(gRenderManager.viewport.layout.type == RenderLayoutTypeRect);
    //=> 1
    _f2(gRenderManager.viewport.size);
    //=> 2560, 1440

    render_manager_destroy();
}

Test(render_manager_size_changed)
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
    GpuDevice *device = gpu_device_create();
    GpuView *view = gpu_view_create(device, visible_frame, &gGpuViewMockConfig);

    render_config.viewport_size = gpu_view_viewport_size(view);

    render_manager_init(&render_config);

    _f2(gRenderManager.viewport.size);
    //=> 1920, 1080

    GpuViewCallbacks view_callbacks = {
        .size_changed = render_manager_size_changed,
    };

    gpu_view_register_callbacks(view, &view_callbacks);
    gpu_view_mock_size_changed(view, new_viewport_size);

    _f2(gRenderManager.viewport.size);
    //=> 2560, 1920

    render_manager_destroy();
    gpu_view_destroy(view);
    gpu_device_destroy(device);
}
