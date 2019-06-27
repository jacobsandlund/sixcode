#ifndef _GpuCommandEncoderMock_h
#define _GpuCommandEncoderMock_h

#include "Gpu/Buffer.h"
#include "Gpu/CommandEncoder.h"
#include "Gpu/PipelineState.h"
#include "Gpu/Primitive.h"

typedef enum {
    GpuCommandEncoderMockStateRenderEncoding,
    GpuCommandEncoderMockStateEndEncoding,
} GpuCommandEncoderMockState;

typedef void (*GpuCommandEncoderMockCallbackFn)(GpuCommandEncoder *encoder);

struct GpuCommandEncoder {
    char *label;
    GpuCommandBuffer *buffer;
    GpuRenderPassConfig *render_pass_config;

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

extern GpuCommandEncoderMockState gGpuCommandEncoderMockState;
extern GpuCommandEncoder gGpuCommandEncoderMock;
extern GpuCommandEncoderMockCallbackFn gGpuCommandEncoderMockCallback;

#endif  // _GpuCommandEncoderMock_h
