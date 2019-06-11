@import MetalKit;

#import "gpu/device.h"
#import "log/manager.h"

void gpu_device_init(GpuDevice *device)
{
    @autoreleasepool {

    id<MTLDevice> mtl_device = MTLCreateSystemDefaultDevice();
    if (!mtl_device) {
        LogDefault(&gLogManager.logs.gpu,
                "Metal is not supported on this device");
        device->device_impl = NULL;
        return;
    }

    device->device_impl = (void *) CFBridgingRetain(mtl_device);

    } // @autoreleasepool
}

void gpu_device_destroy(GpuDevice *device)
{
    CFRelease(device->device_impl);
}
