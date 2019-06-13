#include "gpu/device.h"
#include "test/allocator.h"

struct GpuDevice {
    bool created;
};

GpuDevice *gpu_device_create(void)
{
    GpuDevice *device = tmalloc(sizeof *device);
    device->created = true;
    return device;
}

void gpu_device_destroy(GpuDevice *device)
{
    tfree(device);
}
