#import "gpu/command-encoder.h"

@import MetalKit;

// Order must match GpuPrimitiveType in command-encoder.h
static const i32 MetalPrimitiveTypeLookup[] = {
    MTLPrimitiveTypeTriangle,
};

GpuCommandEncoder *gpu_command_encoder_create(GpuCommandBuffer *buffer, GpuRenderPassConfig *render_pass_config)
{
    GpuCommandEncoder *encoder;

    @autoreleasepool {
        id<MTLCommandBuffer> mtl_buffer = (__bridge id<MTLCommandBuffer>)buffer;
        id<MTLRenderCommandEncoder> mtl_encoder = [mtl_buffer
                renderCommandEncoderWithDescriptor:(__bridge id<MTLRenderPassDescriptor>)render_pass_config];
        encoder = (__bridge_retained GpuCommandEncoder *)mtl_encoder;
    }

    return encoder;
}

void gpu_command_encoder_destroy(GpuCommandEncoder *encoder)
{
    @autoreleasepool {
        id<MTLRenderCommandEncoder> mtl_encoder = (__bridge_transfer id<MTLRenderCommandEncoder>)encoder;
        mtl_encoder = nil;
    }
}

void gpu_command_encoder_label(GpuCommandEncoder *encoder, const char *label)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;
    mtl_encoder.label = label;
}

void gpu_command_encoder_set_viewport(GpuCommandEncoder *encoder, float2 viewport_size)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;

    MTLViewport viewport = {
        0.0,
        0.0,
        _viewport_size.x,
        _viewport_size.y,
        -1.0,
        1.0,
    };
    [mtl_encoder setViewport:viewport];
}

void gpu_command_encoder_set_pipeline_state(GpuCommandEncoder *encoder, GpuPipelineState *state)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;
    id<MTLRenderPipelineState> mtl_state = (__bridge id<MTLRenderPipelineState>)state;

    [mtl_encoder setRenderPipelineState:mtl_state];
}

void gpu_command_encoder_set_vertex_buffer(GpuCommandEncoder *encoder, GpuBuffer *buffer, i32 offset, i32 index)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;
    id<MTLBuffer> mtl_buffer = (__bridge id<MTLBuffer>)buffer;

    [mtl_encoder setVertexBuffer:mtl_buffer offset:offset atIndexindex];
}

void gpu_command_encoder_set_vertex_bytes(GpuCommandEncoder *encoder, void *bytes, i32 length, i32 index)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;

    [mtl_encoder setVertexBytes:bytes length:length atIndex:index];
}

void gpu_command_encoder_draw_primitives(GpuCommandEncoder *encoder, GpuPrimitiveType primitive_type, i64 vertex_start, i64 vertex_count)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;
    i32 mtl_primitive = MetalPrimitiveTypeLookup[primitive_type];

    [mtl_encoder drawPrimitives:mtl_primitive
            vertexStart:vertex_start vertexCount:vertex_count];
}


void gpu_command_encoder_end_encoding(GpuCommandEncoder *encoder)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;

    [mtl_encoder endEncoding];
}
