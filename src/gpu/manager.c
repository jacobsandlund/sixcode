#include "gpu/manager.h"

GpuManager gGpuManager;

void gpu_manager_init(GpuManagerConfig *config, OsScreenFrame visible_frame)
{
    GpuDevice *device = &gGpuManager.device;
    gpu_device_init(device);
    gpu_view_init(&gGpuManager.view, device, visible_frame, &config->view);
    gpu_renderer_init(&gGpuManager.renderer, device, &gGpuManager.view);
}

void gpu_manager_destroy(void)
{
    gpu_renderer_destroy(&gGpuManager.renderer);
    gpu_view_destroy(&gGpuManager.view);
    gpu_device_destroy(&gGpuManager.device);
}

void gpu_manager_draw_in_view(GpuView *view)
{
    gpu_renderer_draw_in_view(&gGpuManager.renderer, view);
}
