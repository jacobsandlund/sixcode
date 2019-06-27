#include "Gpu/CommandEncoderMock.h"

#include <stdlib.h>

GpuCommandEncoder gGpuCommandEncoderMock;

static void GpuCommandEncoderMockDefaultCallback(GpuCommandEncoder *encoder)
{
    gGpuCommandEncoderMock = *encoder;
}

GpuCommandEncoderMockState gGpuCommandEncoderMockState;
GpuCommandEncoderMockCallbackFn gGpuCommandEncoderMockCallback =
        GpuCommandEncoderMockDefaultCallback;

void GpuCommandEncoderBeginRenderEncoding(
        GpuCommandEncoder **encoder_out,
        GpuCommandBuffer *buffer,
        GpuRenderPassConfig *render_pass_config)
{
    GpuCommandEncoder *encoder = malloc(sizeof *encoder);
    *encoder = (GpuCommandEncoder){};
    encoder->buffer = buffer;
    encoder->render_pass_config = render_pass_config;
    *encoder_out = encoder;

    gGpuCommandEncoderMockState = GpuCommandEncoderMockStateRenderEncoding;
}

void GpuCommandEncoderLabel(GpuCommandEncoder *encoder, const char *label)
{
    encoder->label = (char *)label;
}

void GpuCommandEncoderEndEncoding(GpuCommandEncoder *encoder)
{
    (*gGpuCommandEncoderMockCallback)(encoder);
    free(encoder);
    gGpuCommandEncoderMockState = GpuCommandEncoderMockStateEndEncoding;
}
