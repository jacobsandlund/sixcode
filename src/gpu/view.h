#ifndef GpuView_h
#define GpuView_h

#include "spacetime.h"
#include "os/screen.h"
#include "gpu/device.h"
#include "gpu/render-pass-config.h"
#include "gpu/pixel-format.h"

typedef struct GpuView GpuView;

typedef void (*GpuViewFn)(GpuView *view);
typedef void (*GpuViewSizeChangedFn)(GpuView *view, float2 viewport_size);

typedef struct {
    i64 preferred_frames_per_second;
    GpuPixelFormat color_pixel_format;
} GpuViewConfig;

typedef struct {
    GpuViewFn draw_in_view;
    GpuViewSizeChangedFn size_changed;
} GpuViewCallbacks;

GpuView *gpu_view_create(GpuDevice *device, OsScreenFrame frame, GpuViewConfig *config);
void gpu_view_destroy(GpuView *view);
void gpu_view_register_callbacks(GpuView *view, GpuViewCallbacks *callbacks);
float2 gpu_view_viewport_size(GpuView *view);

// Note: this will be the native color pixel format
i64 gpu_view_color_pixel_format(GpuView *view);
GpuRenderPassConfig *gpu_view_current_render_pass_config(GpuView *view);

#endif // GpuView_h
