#import "Gpu/Device.h"

@import MetalKit;

#import <stdlib.h>
#import "Log/Manager.h"

GpuDevice *GpuDeviceCreate(void)
{
    GpuDevice *device;

    @autoreleasepool {

    id<MTLDevice> mtl_device = MTLCreateSystemDefaultDevice();
    if (!mtl_device) {
        LogDefault(gLogManager.logs.gpu,
                "Metal is not supported on this device");
        abort();
        return NULL;
    }

    device = (__bridge_retained GpuDevice *)mtl_device;

    } // @autoreleasepool

    return device;
}

void GpuDeviceDestroy(GpuDevice *device)
{
    @autoreleasepool {
        id<MTLDevice> mtl_device = (__bridge_transfer id<MTLDevice>)device;
        mtl_device = nil;
    }
}
