#import "Gpu/CommandEncoder.h"

@import MetalKit;

void GpuCommandEncoderBeginRenderEncoding(GpuCommandEncoder **encoder, GpuCommandBuffer *buffer, GpuRenderPassConfig *render_pass_config)
{
    @autoreleasepool {
        id<MTLCommandBuffer> mtl_buffer = (__bridge id<MTLCommandBuffer>)buffer;
        id<MTLRenderCommandEncoder> mtl_encoder = [mtl_buffer
                renderCommandEncoderWithDescriptor:(__bridge id<MTLRenderPassDescriptor>)render_pass_config];
        *encoder = (__bridge_retained GpuCommandEncoder *)mtl_encoder;
    }
}

void GpuCommandEncoderLabel(GpuCommandEncoder *encoder, const char *label)
{
    id<MTLCommandEncoder> mtl_encoder = (__bridge id<MTLCommandEncoder>)encoder;
    mtl_encoder.label = label;
}

void GpuCommandEncoderEndEncoding(GpuCommandEncoder *encoder)
{
    @autoreleasepool {
        id<MTLCommandEncoder> mtl_encoder = (__bridge_transfer id<MTLCommandEncoder>)encoder;
        [mtl_encoder endEncoding];
        mtl_encoder = nil;
    }
}
