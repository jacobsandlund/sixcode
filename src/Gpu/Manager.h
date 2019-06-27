#ifndef _GpuManager_h
#define _GpuManager_h

#include "Gpu/CommandQueue.h"
#include "Gpu/Device.h"
#include "Gpu/View.h"
#include "Os/Screen.h"
#include "Spacetime.h"

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
} GpuManager;

extern GpuManager gGpuManager;

void GpuManagerInit(OsScreenFrame frame, GpuManagerConfig *config);
void GpuManagerDestroy(void);
void GpuManagerRegisterCallbacks(GpuManagerCallbacks *callbacks);

#endif  // _GpuManager_h
