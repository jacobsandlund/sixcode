#include "Gpu/Device.h"

#include <stdlib.h>

struct GpuDevice {
    bool created;
};

GpuDevice *GpuDeviceCreate(void)
{
    GpuDevice *device = malloc(sizeof *device);
    device->created = true;
    return device;
}

void GpuDeviceDestroy(GpuDevice *device)
{
    free(device);
}
