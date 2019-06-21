#include "Gpu/CommandEncoderMock.c"

void GpuCmdSetViewport(GpuCommandEncoder *encoder, float2 viewport_size)
{
    encoder->viewport_size = viewport_size;
}

void GpuCmdSetPipelineState(GpuCommandEncoder *encoder, GpuPipelineState *state)
{
    encoder->pipeline_state = state;
}

void GpuCmdBindVertexBuffer(GpuCommandEncoder *encoder, GpuBuffer *buffer, i32 offset, i32 index)
{
    encoder->vertex_buffer = buffer;
    encoder->vertex_buffer_offset = offset;
    encoder->vertex_buffer_index = index;
}

void GpuCmdBindVertexBytes(GpuCommandEncoder *encoder, void *bytes, i32 length, i32 index)
{
    encoder->vertex_bytes = bytes;
    encoder->vertex_bytes_length = length;
    encoder->vertex_bytes_index = index;
}

void GpuCmdDrawPrimitives(GpuCommandEncoder *encoder, GpuPrimitiveType primitive_type, i64 vertex_start, i64 vertex_count)
{
    encoder->draw_primitive_type = primitive_type;
    encoder->draw_vertex_start = vertex_start;
    encoder->draw_vertex_count = vertex_count;
}
