#include "gpu/manager.h"

GpuManager gGpuManager;

void gpu_manager_init(OsScreenFrame frame, GpuManagerConfig *config)
{
    GpuDevice *device = gpu_device_create();
    gGpuManager.device = device;
    gGpuManager.view = gpu_view_create(device, frame, &config->view);
    gGpuManager.renderer = gpu_renderer_create(device, gGpuManager.view);
}

void gpu_manager_destroy(void)
{
    gpu_renderer_destroy(gGpuManager.renderer);
    gpu_view_destroy(gGpuManager.view);
    gpu_device_destroy(gGpuManager.device);
}

void gpu_manager_draw_in_view(GpuView *view)
{
    gpu_renderer_draw_in_view(gGpuManager.renderer, view);
}
