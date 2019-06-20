#ifndef GpuManager_h
#define GpuManager_h

#include "spacetime.h"
#include "os/screen.h"
#include "gpu/device.h"
#include "gpu/view.h"
#include "gpu/renderer.h"
#include "gpu/command-queue.h"

typedef struct {
    GpuViewConfig view;
} GpuManagerConfig;

typedef struct {
    GpuViewCallbacks view;
} GpuManagerCallbacks;

typedef struct {
    GpuDevice *device;
    GpuView *view;
    GpuCommandQueue *command_queue;
    GpuRenderer renderer;
} GpuManager;

extern GpuManager gGpuManager;

void gpu_manager_init(OsScreenFrame frame, GpuManagerConfig *config);
void gpu_manager_destroy(void);
void gpu_manager_register_callbacks(GpuManagerCallbacks *callbacks);
void gpu_manager_draw_in_view(GpuView *view);
void gpu_manager_size_changed(GpuView *view, float2 viewport_size);

#endif // GpuManager_h
