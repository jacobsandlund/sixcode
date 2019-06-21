#ifndef _GpuCmd_h
#define _GpuCmd_h

#include "spacetime.h"
#include "gpu/buffer.h"
#include "gpu/command-encoder.h"
#include "gpu/pipeline-state.h"
#include "gpu/primitive.h"

void GpuCmdSetViewport(GpuCommandEncoder *encoder, float2 viewport_size);
void GpuCmdSetPipelineState(GpuCommandEncoder *encoder, GpuPipelineState *state);
void GpuCmdBindVertexBuffer(GpuCommandEncoder *encoder, GpuBuffer *buffer, i32 offset, i32 index);
void GpuCmdBindVertexBytes(GpuCommandEncoder *encoder, void *bytes, i32 length, i32 index);

void GpuCmdDrawPrimitives(GpuCommandEncoder *encoder, GpuPrimitiveType primitive_type, i64 vertex_start, i64 vertex_count);

#endif // _GpuCmd_h
