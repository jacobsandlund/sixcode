#import "Gpu/CommandBuffer.h"

@import MetalKit;

GpuCommandBuffer *GpuCommandBufferCreate(GpuCommandQueue *queue)
{
    GpuCommandBuffer *buffer;

    @autoreleasepool {
        id<MTLCommandQueue> mtl_queue = (__bridge id<MTLCommandQueue>)queue;
        id<MTLCommandBuffer> mtl_buffer = [mtl_queue commandBuffer];
        buffer = (__bridge_retained GpuCommandBuffer *)mtl_buffer;
    }

    return buffer;
}

void GpuCommandBufferDestroy(GpuCommandBuffer *buffer)
{
    @autoreleasepool {
        id<MTLCommandBuffer> mtl_buffer =
                (__bridge_transfer id<MTLCommandBuffer>)buffer;
        mtl_buffer = nil;
    }
}

void GpuCommandBufferPresentDrawable(GpuCommandBuffer *buffer, GpuView *view)
{
    id<MTLCommandBuffer> mtl_buffer = (__bridge id<MTLCommandBuffer>)buffer;
    ViewDelegate *view_delegate = (__bridge ViewDelegate *)view;
    MTKView *mtk_view = view_delegate.mtk_view;
    [mtl_buffer presentDrawable:mtk_view.currentDrawable];
}

void GpuCommandBufferCommit(GpuCommandBuffer *buffer)
{
    id<MTLCommandBuffer> mtl_buffer = (__bridge id<MTLCommandBuffer>)buffer;
    [mtl_buffer commit];
}
