@import MetalKit

#import "gpu/device.h"

void gpu_device_init(GpuDevice *device)
{
    MTLDevice mtl_device = MTLCreateSystemDefaultDevice();
    if (!mtl_device) {
        Log("Metal is not supported on this device");
        device->device_impl = NULL;
        return;
    }

    device->device_impl = CFBridgingRetain(mtl_device);
}

void gpu_device_destroy(GpuDevice *device)
{
    CFRelease(device->device_impl);
}
