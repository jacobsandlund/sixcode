#ifndef GpuView_h
#define GpuView_h

#include "spacetime.h"
#include "gpu/device.h"

typedef struct {
    void *view_impl;
    float2 viewport_size;
} GpuView;

typedef void (*GpuViewFn)(GpuView *view);

typedef struct {
    GpuViewFn size_changed;
    GpuViewFn draw_in_view;
    i64 preferred_frames_per_second;
} GpuViewConfig;

void gpu_view_init(GpuView *view, GpuDevice *device, float2 viewport_size, GpuViewConfig *config);
void gpu_view_destroy(GpuView *view);

#endif // GpuView_h
