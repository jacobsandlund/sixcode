#include "gpu/device.h"
#include <stdlib.h>

struct GpuDevice {
    bool created;
};

GpuDevice *gpu_device_create(void)
{
    GpuDevice *device = malloc(sizeof *device);
    device->created = true;
    return device;
}

void gpu_device_destroy(GpuDevice *device)
{
    free(device);
}
