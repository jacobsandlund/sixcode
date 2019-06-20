#ifndef GpuRenderer_h
#define GpuRenderer_h

#include "spacetime.h"
#include "gpu/buffer.h"
#include "gpu/command-buffer.h"
#include "gpu/command-encoder.h"
#include "gpu/command-queue.h"
#include "gpu/device.h"
#include "gpu/function.h"
#include "gpu/pipeline-state.h"
#include "gpu/view.h"

typedef struct {
    float2 viewport_size;
    GpuFunction *vertex_function;
    GpuFunction *fragment_function;
    GpuPipelineState *pipeline_state;
    GpuBuffer *vertex_buffer;
    i64 num_vertices;
} GpuRenderer;

void gpu_renderer_init(GpuRenderer *renderer, GpuDevice *device, GpuView *view, float2 viewport_size);
void gpu_renderer_destroy(GpuRenderer *renderer);
void gpu_renderer_draw_in_view(GpuRenderer *renderer, GpuView *view, GpuCommandQueue *queue);
void gpu_renderer_size_changed(GpuRenderer *renderer, GpuView *view, float2 viewport_size);

#endif // GpuRenderer_h
