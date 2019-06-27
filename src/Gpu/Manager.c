#include "Gpu/Manager.h"

GpuManager gGpuManager;

void GpuManagerInit(OsScreenFrame frame, GpuManagerConfig *config)
{
    GpuDevice *device = GpuDeviceCreate();
    gGpuManager.device = device;
    gGpuManager.view = GpuViewCreate(device, frame, &config->view);
    gGpuManager.command_queue = GpuCommandQueueCreate(device);
}

void GpuManagerDestroy(void)
{
    GpuCommandQueueDestroy(gGpuManager.command_queue);
    GpuViewDestroy(gGpuManager.view);
    GpuDeviceDestroy(gGpuManager.device);
}

void GpuManagerRegisterCallbacks(GpuManagerCallbacks *callbacks)
{
    GpuViewRegisterCallbacks(gGpuManager.view, &callbacks->view);
}
