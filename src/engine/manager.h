#ifndef EngineManager_h
#define EngineManager_h

#include "spacetime.h"
#include "log/manager.h"
#include "os/manager.h"
#include "gpu/manager.h"
#include "string/manager.h"
#include "resource/manager.h"
#include "render/manager.h"
#include "camera/manager.h"
#include "world/manager.h"

typedef struct {
    LogManagerConfig log;
    OsManagerConfig os;
    GpuManagerConfig gpu;
    StringManagerConfig string;
    ResourceManagerConfig resource;
    RenderManagerConfig render;
    CameraManagerConfig camera;
    WorldManagerConfig world;
    i64 world_grid_random_count;
} EngineConfig;

typedef struct {
    OsManagerCallbacks os;
    GpuManagerCallbacks gpu;
} EngineCallbacks;

extern EngineConfig gEngineConfig;
extern EngineCallbacks gEngineCallbacks;

void engine_manager_init(EngineCallbacks *callbacks, EngineConfig *config);
void engine_manager_destroy(void);
void engine_manager_run(void);

#endif // EngineManager_h
