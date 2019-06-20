#include "gpu/command-encoder.h"
#include <stdlib.h>

struct GpuCommandEncoder {
    GpuCommandBuffer *buffer;
    GpuRenderPassConfig render_pass_config;
    char *label;

    float2 viewport_size;
    GpuPipelineState *pipeline_state;
    GpuBuffer *vertex_buffer;
    i32 vertex_buffer_offset;
    i32 vertex_buffer_index;
    void *vertex_bytes;
    i32 vertex_bytes_length;
    i32 vertex_bytes_index;

    GpuPrimitiveType draw_primitive_type;
    i64 draw_vertex_start;
    i64 draw_vertex_count;

    bool encoding_ended;
};

GpuCommandEncoder *gpu_command_encoder_create(GpuCommandBuffer *buffer, GpuRenderPassConfig *render_pass_config)
{
    GpuCommandEncoder *encoder = malloc(sizeof *encoder);

    *encoder = (GpuCommandEncoder) {};
    encoder->buffer = buffer;
    encoder->render_pass_config = render_pass_config;

    return encoder;
}

void gpu_command_encoder_destroy(GpuCommandEncoder *encoder)
{
    free(encoder);
}

void gpu_command_encoder_label(GpuCommandEncoder *encoder, const char *label)
{
    encoder->label = (char *) label;
}

void gpu_command_encoder_set_viewport(GpuCommandEncoder *encoder, float2 viewport_size)
{
    encoder->viewport_size = viewport_size;
}

void gpu_command_encoder_set_pipeline_state(GpuCommandEncoder *encoder, GpuPipelineState *state)
{
    encoder->pipeline_state = state;
}

void gpu_command_encoder_set_vertex_buffer(GpuCommandEncoder *encoder, GpuBuffer *buffer, i32 offset, i32 index)
{
    encoder->vertex_buffer = buffer;
    encoder->vertex_buffer_offset = offset;
    encoder->vertex_buffer_index = index;
}

void gpu_command_encoder_set_vertex_bytes(GpuCommandEncoder *encoder, void *bytes, i32 length, i32 index)
{
    encoder->vertex_bytes = bytes;
    encoder->vertex_bytes_length = length;
    encoder->vertex_bytes_index = index;
}

void gpu_command_encoder_draw_primitives(GpuCommandEncoder *encoder, GpuPrimitiveType primitive_type, i64 vertex_start, i64 vertex_count)
{
    encoder->draw_primitive_type = primitive_type;
    encoder->draw_vertex_start = vertex_start;
    encoder->draw_vertex_count = vertex_count;
}

void gpu_command_encoder_end_encoding(GpuCommandEncoder *encoder)
{
    encoder->encoding_ended = true;
}
