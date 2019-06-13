#include "gpu/device-mock.c"
#include "test/test.h"

Test(gpu_device)
{
    GpuDevice *device = gpu_device_create();

    _d(device->created);
    //=> 1

    gpu_device_destroy(device);

    _d(test_allocator_was_freed(device));
    //=> 1
}
