#include "Gpu/CommandEncoder.h"

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
};

void GpuCommandEncoderBeginRenderEncoding(
        GpuCommandEncoder **encoder,
        GpuCommandBuffer *buffer,
        GpuRenderPassConfig *render_pass_config)
{
    *encoder = malloc(sizeof *encoder);

    **encoder = (GpuCommandEncoder){};
    encoder->buffer = buffer;
    encoder->render_pass_config = render_pass_config;
}

void GpuCommandEncoderLabel(GpuCommandEncoder *encoder, const char *label)
{
    encoder->label = (char *)label;
}

void GpuCommandEncoderEndEncoding(GpuCommandEncoder *encoder)
{
    // TODO: callback to get result of encoding for testing purposes.
    free(encoder);
}
