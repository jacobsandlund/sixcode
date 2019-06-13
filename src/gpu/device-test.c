#include "gpu/device-mock.c"
#include "test.h"

Test(gpu_device)
{
    GpuDevice *device = gpu_device_create();

    _d(device->created);
    //=> 1

    gpu_device_destroy(device);
}
