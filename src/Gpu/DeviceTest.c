#include "Gpu/DeviceMock.c"
#include "Test.h"

Test(gpu_device)
{
    GpuDevice *device = GpuDeviceCreate();

    _d(device->created);
    //=> 1

    GpuDeviceDestroy(device);
}
