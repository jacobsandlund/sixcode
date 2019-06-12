#ifndef GpuRenderer_h
#define GpuRenderer_h

#include "spacetime.h"
#include "gpu/device.h"
#include "gpu/view.h"

typedef struct GpuRenderer GpuRenderer;

GpuRenderer *gpu_renderer_create(GpuDevice *device, GpuView *view);
void gpu_renderer_destroy(GpuRenderer *renderer);
void gpu_renderer_draw_in_view(GpuRenderer *renderer, GpuView *view);

#endif // GpuRenderer_h
