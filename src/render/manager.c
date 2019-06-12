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

void render_manager_size_changed(GpuView *view, float2 viewport_size)
{
    (void) view;
    gRenderManager.viewport.size = viewport_size;
}
