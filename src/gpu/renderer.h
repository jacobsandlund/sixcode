#ifndef _GpuRenderer_h
#define _GpuRenderer_h

#include "spacetime.h"
#include "gpu/buffer.h"
#include "gpu/command-buffer.h"
#include "gpu/command-encoder.h"
#include "gpu/command-queue.h"
#include "gpu/device.h"
#include "gpu/function.h"
#include "gpu/pipeline-state.h"
#include "gpu/pixel-format.h"
#include "gpu/view.h"

typedef struct {
    float2 viewport_size;
    GpuPixelFormat pixel_format;
} GpuRendererConfig;

typedef struct {
    float2 viewport_size;
    GpuFunction *vertex_function;
    GpuFunction *fragment_function;
    GpuPipelineState *pipeline_state;
    GpuBuffer *vertex_buffer;
    i64 num_vertices;
} GpuRenderer;

void GpuRendererInit(GpuRenderer *renderer, GpuDevice *device, GpuView *view, GpuRendererConfig *config);
void GpuRendererDestroy(GpuRenderer *renderer);
void GpuRendererDrawInView(GpuRenderer *renderer, GpuView *view, GpuCommandQueue *queue);
void GpuRendererSizeChanged(GpuRenderer *renderer, GpuView *view, float2 viewport_size);

#endif // _GpuRenderer_h
