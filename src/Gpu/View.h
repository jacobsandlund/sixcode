#ifndef _GpuView_h
#define _GpuView_h

#include "Gpu/Device.h"
#include "Gpu/PixelFormat.h"
#include "Gpu/RenderPassConfig.h"
#include "Os/Screen.h"
#include "Spacetime.h"

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

GpuView *GpuViewCreate(GpuDevice *device,
                       OsScreenFrame frame,
                       GpuViewConfig *config);
void GpuViewDestroy(GpuView *view);
void GpuViewRegisterCallbacks(GpuView *view, GpuViewCallbacks *callbacks);
float2 GpuViewViewportSize(GpuView *view);
GpuRenderPassConfig *GpuViewCurrentRenderPassConfig(GpuView *view);

#endif  // _GpuView_h
