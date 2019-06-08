@import MetalKit

#import "gpu/device.h"
#import "log/manager.h"

void gpu_device_init(GpuDevice *device)
{
    MTLDevice mtl_device = MTLCreateSystemDefaultDevice();
    if (!mtl_device) {
        log_at_level(&gLogManager.logs.gpu, LogLevelDefault,
                "Metal is not supported on this device");
        device->device_impl = NULL;
        return;
    }

    device->device_impl = CFBridgingRetain(mtl_device);
}

void gpu_device_destroy(GpuDevice *device)
{
    CFRelease(device->device_impl);
}
