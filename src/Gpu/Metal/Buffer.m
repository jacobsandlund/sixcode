#import "Gpu/Buffer.h"

@import MetalKit;

// Order must match GpuBufferStorageMode in buffer.h
static const i32 MetalResourceStorageModeLookup[] = {
    MTLResourceStorageModeShared,
};

GpuBuffer *GpuBufferCreateWithLength(GpuDevice *device, i64 length, GpuBufferStorageMode storage_mode)
{
    GpuBuffer *buffer;

    @autoreleasepool {
        id<MTLDevice> mtl_device = (__bridge id<MTLDevice>)device;
        i32 options = MetalResourceStorageModeLookup[storage_mode];
        id<MTLBuffer> mtl_buffer = [mtl_device newBufferWithLength:length
                options:options];
        buffer = (__bridge_retained GpuBuffer *)mtl_buffer;
    }

    return buffer;
}

void GpuBufferDestroy(GpuBuffer *buffer)
{
    @autoreleasepool {
        id<MTLBuffer> mtl_buffer = (__bridge_transfer GpuBuffer *)buffer;
        mtl_buffer = nil;
    }
}

void *GpuBufferContents(GpuBuffer *buffer)
{
    id<MTLBuffer> mtl_buffer = (__bridge GpuBuffer *)buffer;
    return mtl_buffer.contents;
}
