#include "test.h"
#include "render/manager.c"
#include "render/layout.c"

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
