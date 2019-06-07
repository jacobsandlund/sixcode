#ifndef WorldManager_h
#define WorldManager_h

#include "spacetime.h"
#include "world/grid.h"

typedef struct {
    WorldGrid grid;
} WorldManager;

extern WorldManager gWorldManager;

void world_manager_init(void);
void world_manager_destroy(void);
void world_manager_load_random(i64 count);
//void world_manager_update(void);

#endif // WorldManager_h
