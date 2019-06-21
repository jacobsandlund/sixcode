#import "gpu/cmd.h"

@import MetalKit;

#import "gpu/metal/primitive.h"

void GpuCmdSetViewport(GpuCommandEncoder *encoder, float2 viewport_size)
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

void GpuCmdSetPipelineState(GpuCommandEncoder *encoder, GpuPipelineState *state)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;
    id<MTLRenderPipelineState> mtl_state = (__bridge id<MTLRenderPipelineState>)state;

    [mtl_encoder setRenderPipelineState:mtl_state];
}

void GpuCmdBindVertexBuffer(GpuCommandEncoder *encoder, GpuBuffer *buffer, i32 offset, i32 index)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;
    id<MTLBuffer> mtl_buffer = (__bridge id<MTLBuffer>)buffer;

    [mtl_encoder setVertexBuffer:mtl_buffer offset:offset atIndexindex];
}

void GpuCmdBindVertexBytes(GpuCommandEncoder *encoder, void *bytes, i32 length, i32 index)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;

    [mtl_encoder setVertexBytes:bytes length:length atIndex:index];
}

void GpuCmdDrawPrimitives(GpuCommandEncoder *encoder, GpuPrimitiveType primitive_type, i64 vertex_start, i64 vertex_count)
{
    id<MTLRenderCommandEncoder> mtl_encoder = (__bridge id<MTLRenderCommandEncoder>)encoder;
    i32 mtl_primitive = MetalPrimitiveTypeLookup[primitive_type];

    [mtl_encoder drawPrimitives:mtl_primitive
            vertexStart:vertex_start vertexCount:vertex_count];
}
