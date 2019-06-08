#include "test.h"
#include "render/manager.c"
#include "render/layout.c"
#include "gpu/view-mock.c"

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

    GpuView *view = tmalloc(sizeof *view);
    GpuViewConfig view_config = {
        .size_changed = render_manager_size_changed,
    };

    gpu_view_init(view, NULL, old_viewport_size, &view_config);

    // Note: gRenderManager.viewport.size is now correct,
    // but prefer explicit initialization
    _f2(gRenderManager.viewport.size);
    //=> 1920, 1080
    render_config.viewport_size = view->viewport_size;

    render_manager_init(&render_config);

    _f2(gRenderManager.viewport.size);
    //=> 1920, 1080

    gpu_view_mock_size_changed(view, new_viewport_size);

    _f2(gRenderManager.viewport.size);
    //=> 2560, 1920

    render_manager_destroy();
}
