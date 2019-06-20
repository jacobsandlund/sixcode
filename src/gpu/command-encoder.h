#ifndef GpuCommandEncoder_h
#define GpuCommandEncoder_h

#include "spacetime.h"
#include "gpu/render-pass-config.h"
#include "gpu/command-buffer.h"
#include "gpu/pipeline-state.h"
#include "gpu/buffer.h"

typedef enum {
    GpuPrimitiveTypeTriangle,
} GpuPrimitiveType;

typedef struct GpuCommandEncoder GpuCommandEncoder;

GpuCommandEncoder *gpu_command_encoder_create(GpuCommandBuffer *buffer, GpuRenderPassConfig *render_pass_config);
void gpu_command_encoder_destroy(GpuCommandEncoder *encoder);

void gpu_command_encoder_label(GpuCommandEncoder *encoder, const char *label);

void gpu_command_encoder_set_viewport(GpuCommandEncoder *encoder, float2 viewport_size);
void gpu_command_encoder_set_pipeline_state(GpuCommandEncoder *encoder, GpuPipelineState *state);
void gpu_command_encoder_set_vertex_buffer(GpuCommandEncoder *encoder, GpuBuffer *buffer, i32 offset, i32 index);
void gpu_command_encoder_set_vertex_bytes(GpuCommandEncoder *encoder, void *bytes, i32 length, i32 index);

void gpu_command_encoder_draw_primitives(GpuCommandEncoder *encoder, GpuPrimitiveType primitive_type, i64 vertex_start, i64 vertex_count);

void gpu_command_encoder_end_encoding(GpuCommandEncoder *encoder);

#endif // GpuCommandEncoder_h
