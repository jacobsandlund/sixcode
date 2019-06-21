#include "gpu/manager.h"

GpuManager gGpuManager;

void GpuManagerInit(OsScreenFrame frame, GpuManagerConfig *config)
{
    GpuDevice *device = GpuDeviceCreate();
    gGpuManager.device = device;
    gGpuManager.view = GpuViewCreate(device, frame, &config->view);
    gGpuManager.command_queue = GpuCommandQueueCreate(device);

    GpuRendererConfig renderer_config = {
        .viewport_size = GpuViewViewportSize(gGpuManager.view),
        .pixel_format = config->view.color_pixel_format,
    };
    GpuRendererInit(&gGpuManager.renderer, device, gGpuManager.view, &renderer_config);
}

void GpuManagerDestroy(void)
{
    GpuRendererDestroy(&gGpuManager.renderer);
    GpuCommandQueueDestroy(gGpuManager.command_queue);
    GpuViewDestroy(gGpuManager.view);
    GpuDeviceDestroy(gGpuManager.device);
}

void GpuManagerRegisterCallbacks(GpuManagerCallbacks *callbacks)
{
    GpuViewRegisterCallbacks(gGpuManager.view, &callbacks->view);
}

void GpuManagerDrawInView(GpuView *view)
{
    GpuRendererDrawInView(&gGpuManager.renderer, view, gGpuManager.command_queue);
}

void GpuManagerSizeChanged(GpuView *view, float2 viewport_size)
{
    GpuRendererSizeChanged(&gGpuManager.renderer, view, viewport_size);
}
