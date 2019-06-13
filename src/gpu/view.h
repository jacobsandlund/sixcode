#ifndef GpuView_h
#define GpuView_h

#include "spacetime.h"
#include "os/screen.h"
#include "gpu/device.h"

typedef struct GpuView GpuView;

typedef void (*GpuViewFn)(GpuView *view);
typedef void (*GpuViewSizeChangedFn)(GpuView *view, float2 viewport_size);

typedef struct {
    GpuViewFn draw_in_view;
    GpuViewSizeChangedFn size_changed;
    i64 preferred_frames_per_second;
} GpuViewConfig;

GpuView *gpu_view_create(GpuDevice *device, OsScreenFrame frame, GpuViewConfig *config);
void gpu_view_destroy(GpuView *view);
float2 gpu_view_viewport_size(GpuView *view);

#endif // GpuView_h
