#include "gpu/device-mock.c"
#include "test.h"

Test(gpu_device)
{
    GpuDevice *device = GpuDeviceCreate();

    _d(device->created);
    //=> 1

    GpuDeviceDestroy(device);
}
