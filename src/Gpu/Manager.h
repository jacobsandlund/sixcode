#ifndef _GpuManager_h
#define _GpuManager_h

#include "Spacetime.h"
#include "Os/Screen.h"
#include "Gpu/Device.h"
#include "Gpu/View.h"
#include "Gpu/Renderer.h"
#include "Gpu/CommandQueue.h"

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
