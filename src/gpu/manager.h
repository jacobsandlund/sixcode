#ifndef _GpuManager_h
#define _GpuManager_h

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

void GpuManagerInit(OsScreenFrame frame, GpuManagerConfig *config);
void GpuManagerDestroy(void);
void GpuManagerRegisterCallbacks(GpuManagerCallbacks *callbacks);
void GpuManagerDrawInView(GpuView *view);
void GpuManagerSizeChanged(GpuView *view, float2 viewport_size);

#endif // _GpuManager_h
