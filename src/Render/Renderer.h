#ifndef _Renderer_h
#define _Renderer_h

#include "Gpu/Buffer.h"
#include "Gpu/CommandBuffer.h"
#include "Gpu/CommandEncoder.h"
#include "Gpu/CommandQueue.h"
#include "Gpu/Device.h"
#include "Gpu/Function.h"
#include "Gpu/PipelineState.h"
#include "Gpu/PixelFormat.h"
#include "Gpu/View.h"
#include "Render/Viewport.h"
#include "Spacetime.h"

typedef struct {
    GpuViewConfig view;
} RendererConfig;

typedef struct {
    GpuFunction *vertex_function;
    GpuFunction *fragment_function;
    GpuPipelineState *pipeline_state;
    GpuBuffer *vertex_buffer;
    i64 num_vertices;
} Renderer;

void RendererInit(Renderer *renderer,
                  GpuDevice *device,
                  RendererConfig *config);
void RendererDestroy(Renderer *renderer);
void RendererDrawInView(Renderer *renderer,
                        GpuView *view,
                        RenderViewport *viewport,
                        GpuCommandQueue *queue);

#endif  // _Renderer_h
