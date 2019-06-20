#import "gpu/function.h"

@import MetalKit;

GpuFunction *gpu_function_create_with_name(GpuDevice *device, const char *name)
{
    GpuFunction *function;

    @autoreleasepool {
        id<MTLDevice> mtl_device = (__bridge id<MTLDevice>)device;
        id<MTLLibrary> defaultLibrary = [mtl_device newDefaultLibrary];
        id<MTLFunction> mtl_function = [defaultLibrary newFunctionWithName:name];
        function = (__bridge_retained GpuFunction *)mtl_function;
    }

    return function;
}

void gpu_function_destroy(GpuFunction *function)
{
    @autoreleasepool {
        id<MTLFunction> mtl_function = (__bridge_transfer id<MTLFunction>)function;
        mtl_function = nil;
    }
}
