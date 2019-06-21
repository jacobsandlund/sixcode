#include "Render/Manager.h"
#include "Render/Layout.h"

RenderManager gRenderManager;

void RenderManagerInit(RenderManagerConfig *config)
{
    RenderLayoutSetType(&gRenderManager.viewport.layout, config->layout_type);
    gRenderManager.viewport.size = config->viewport_size;
}

void RenderManagerDestroy(void)
{
    // Do nothing
}

void RenderManagerSizeChanged(GpuView *view, float2 viewport_size)
{
    (void) view;
    gRenderManager.viewport.size = viewport_size;
}
