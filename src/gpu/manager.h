#ifndef GpuManager_h
#define GpuManager_h

#include "spacetime.h"
#include "gpu/device.h"
#include "gpu/view.h"
#include "gpu/renderer.h"

typedef struct {
    GpuViewConfig view;
} GpuManagerConfig;

typedef struct {
    GpuDevice device;
    GpuView view;
    GpuRenderer renderer;
} GpuManager;

extern GpuManager gGpuManager;

void gpu_manager_init(GpuManagerConfig *config, float2 viewport_size);
void gpu_manager_destroy(void);
void gpu_manager_draw_in_view(GpuView *view);

#endif // GpuManager_h
