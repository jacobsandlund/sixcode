#ifndef _GpuCommandEncoder_h
#define _GpuCommandEncoder_h

#include "spacetime.h"
#include "gpu/render-pass-config.h"
#include "gpu/command-buffer.h"

typedef struct GpuCommandEncoder GpuCommandEncoder;

void GpuCommandEncoderBeginRenderEncoding(GpuCommandEncoder **encoder, GpuCommandBuffer *buffer, GpuRenderPassConfig *render_pass_config);
// void GpuCommandEncoderBeginCopyEncoding(GpuCommandBuffer *buffer);
// void GpuCommandEncoderBeginComputeEncoding(GpuCommandBuffer *buffer);

void GpuCommandEncoderLabel(GpuCommandEncoder *encoder, const char *label);

void GpuCommandEncoderEndEncoding(GpuCommandEncoder *encoder);

#endif // _GpuCommandEncoder_h
