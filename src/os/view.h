#ifndef OsView_h
#define OsView_h

#include "spacetime.h"
#include "gpu/pixel-format.h"

typedef struct {
    GpuPixelFormat color_pixel_format;
    float4 clear_color;
    i64 preferred_frames_per_second;
} OsViewConfig;

#endif // OsView_h
