#ifndef _GpuRenderer_h
#define _GpuRenderer_h

#include "Gpu/Buffer.h"
#include "Gpu/CommandBuffer.h"
#include "Gpu/CommandEncoder.h"
#include "Gpu/CommandQueue.h"
#include "Gpu/Device.h"
#include "Gpu/Function.h"
#include "Gpu/PipelineState.h"
#include "Gpu/PixelFormat.h"
#include "Gpu/View.h"
#include "spacetime.h"

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

void GpuRendererInit(GpuRenderer *renderer,
                     GpuDevice *device,
                     GpuView *view,
                     GpuRendererConfig *config);
void GpuRendererDestroy(GpuRenderer *renderer);
void GpuRendererDrawInView(GpuRenderer *renderer,
                           GpuView *view,
                           GpuCommandQueue *queue);
void GpuRendererSizeChanged(GpuRenderer *renderer,
                            GpuView *view,
                            float2 viewport_size);

#endif  // _GpuRenderer_h
