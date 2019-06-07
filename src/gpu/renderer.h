#ifndef GpuRenderer_h
#define GpuRenderer_h

typedef struct {
    void *renderer_impl;
    GpuDevice *device;
} GpuRenderer;

void gpu_renderer_init(GpuRenderer *renderer, GpuDevice *device, GpuView *view);
void gpu_renderer_destroy(GpuRenderer *renderer);
void gpu_renderer_draw_in_view(GpuRenderer *renderer, GpuView *view);

#endif // GpuRenderer_h
