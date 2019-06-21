#ifndef _EngineManager_h
#define _EngineManager_h

#include "spacetime.h"
#include "log/manager.h"
#include "os/manager.h"
#include "gpu/manager.h"
#include "render/manager.h"
#include "camera/manager.h"
#include "world/manager.h"

typedef struct {
    LogManagerConfig log;
    OsManagerConfig os;
    GpuManagerConfig gpu;
    RenderManagerConfig render;
    CameraManagerConfig camera;
    WorldManagerConfig world;
    i64 world_grid_random_count;
} EngineManagerConfig;

typedef struct {
    OsManagerCallbacks os;
    GpuManagerCallbacks gpu;
} EngineManagerCallbacks;

extern EngineManagerConfig gEngineManagerConfig;
extern EngineManagerCallbacks gEngineManagerCallbacks;

void EngineManagerInit(EngineManagerCallbacks *callbacks, EngineManagerConfig *config);
void EngineManagerDestroy(void);
void EngineManagerRun(void);

#endif // _EngineManager_h
