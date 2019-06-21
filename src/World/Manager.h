#ifndef _WorldManager_h
#define _WorldManager_h

#include "Spacetime.h"
#include "World/Grid.h"

typedef struct {
    i32 grid_size;
} WorldManagerConfig;

typedef struct {
    WorldGrid grid;
} WorldManager;

extern WorldManager gWorldManager;

void WorldManagerInit(WorldManagerConfig *config);
void WorldManagerDestroy(void);
void WorldManagerLoadRandom(i64 count);
//void WorldManagerUpdate(void);

#endif // _WorldManager_h
