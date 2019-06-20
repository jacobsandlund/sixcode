#import "gpu/command-queue.h"

@import MetalKit;

GpuCommandQueue *gpu_command_queue_create(GpuDevice *device)
{
    GpuCommandQueue *queue;

    @autoreleasepool {
        id<MTLDevice> mtl_device = (__bridge id<MTLDevice>)device;
        id<MTLCommandQueue> mtl_queue = [mtl_device newCommandQueue];
        queue = (__bridge_retained GpuCommandQueue *)mtl_queue;
    }

    return queue;
}

void gpu_command_queue_destroy(GpuCommandQueue *queue)
{
    @autoreleasepool {
        id<MTLCommandQueue> mtl_queue = (__bridge_transfer id<MTLCommandQueue>)queue;
        mtl_queue = nil;
    }
}
