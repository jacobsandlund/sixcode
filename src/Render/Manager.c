#include "Render/Manager.h"

#include "Gpu/Manager.h"

RenderManager gRenderManager;

void RenderManagerInit(RenderManagerConfig *config)
{
    RenderViewportLayout(&gRenderManager.viewport, config->layout_type);
    gRenderManager.viewport.size = GpuViewViewportSize(gGpuManager.view);

    RendererInit(
            &gRenderManager.renderer, gGpuManager.device, &config->renderer);
}

void RenderManagerDestroy(void)
{
    RendererDestroy(&gRenderManager.renderer);
}

void RenderManagerDrawInView(GpuView *view)
{
    RendererDrawInView(&gRenderManager.renderer,
                       view,
                       &gRenderManager.viewport,
                       gGpuManager.command_queue);
}

void RenderManagerSizeChanged(GpuView *view, float2 viewport_size)
{
    (void)view;
    gRenderManager.viewport.size = viewport_size;
}
