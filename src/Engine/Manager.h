#ifndef _EngineManager_h
#define _EngineManager_h

#include "Camera/Manager.h"
#include "Gpu/Manager.h"
#include "Log/Manager.h"
#include "Os/Manager.h"
#include "Render/Manager.h"
#include "Spacetime.h"
#include "World/Manager.h"

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

void EngineManagerInit(EngineManagerCallbacks *callbacks,
                       EngineManagerConfig *config);
void EngineManagerDestroy(void);
void EngineManagerRun(void);

#endif  // _EngineManager_h
