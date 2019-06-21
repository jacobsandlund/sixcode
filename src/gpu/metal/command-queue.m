#import "gpu/command-queue.h"

@import MetalKit;

GpuCommandQueue *GpuCommandQueueCreate(GpuDevice *device)
{
    GpuCommandQueue *queue;

    @autoreleasepool {
        id<MTLDevice> mtl_device = (__bridge id<MTLDevice>)device;
        id<MTLCommandQueue> mtl_queue = [mtl_device newCommandQueue];
        queue = (__bridge_retained GpuCommandQueue *)mtl_queue;
    }

    return queue;
}

void GpuCommandQueueDestroy(GpuCommandQueue *queue)
{
    @autoreleasepool {
        id<MTLCommandQueue> mtl_queue = (__bridge_transfer id<MTLCommandQueue>)queue;
        mtl_queue = nil;
    }
}
