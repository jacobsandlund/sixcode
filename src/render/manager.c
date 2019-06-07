#include "render/manager.h"
#include "render/layout.h"

RenderManager gRenderManager;

void render_manager_init(RenderManagerConfig *config)
{
    render_layout_type(&gRenderManager.viewport.layout, config->layout_type);
    gRenderManager.viewport.size = config->viewport_size;
}

void render_manager_destroy(void)
{
    // Do nothing
}
