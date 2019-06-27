#include "Gpu/RenderPassConfig.h"

@import MetalKit;

void GpuRenderPassConfigDestroy(GpuRenderPassConfig *config)
{
    @autoreleasepool {
        MTLRenderPassDescriptor *descriptor =
                (__bridge_transfer MTLRenderPassDescriptor *)config;
        descriptor = nil;
    }
}
