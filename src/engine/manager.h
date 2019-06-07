#ifndef EngineManager_h
#define EngineManager_h

#include "spacetime.h"
#include "os/manager.h"

typedef struct {
    OsManagerConfig os;
} EngineConfig;

void engine_manager_init(EngineConfig *config);
void engine_manager_destroy(void);

#endif // EngineManager_h
