#include "World/Manager.h"

#include "Os/Clock.h"

#include <math.h>
#include <stdlib.h>

#define WorldManagerHexFillMin 1
#define WorldManagerHexFillMax 15

WorldManager gWorldManager;

void WorldManagerInit(WorldManagerConfig *config)
{
    WorldGridInit(&gWorldManager.grid, config->grid_size);
}

void WorldManagerDestroy(void)
{
    WorldGridDestroy(&gWorldManager.grid);
}

void WorldManagerLoadRandom(i64 count)
{
    srand((unsigned int)OsClockTime());
    i64 style = 0;

    WorldGrid *g = &gWorldManager.grid;

    int2 size = {
        g->size_quad.size.x,
        g->size_quad.size.y,
    };
    int2 half_size = {
        size.x / 2,
        size.y / 2,
    };

    for (i64 i = 0; i < count; i++) {
        double rand1 = (double)rand() / (double)RAND_MAX;
        double rand2 = (double)rand() / (double)RAND_MAX;
        int2 h = {
            floor(rand1 * size.x) - half_size.x,
            floor(rand2 * size.y) - half_size.y,
        };

        style++;
        if (style == 16) {
            style = 1;
        }

        WorldGridSet(g, h, style);
    }
}

/*
void WorldManagerUpdate(void)
{
    static u8 style = WorldManagerHexFillMin;

    ivec2 h = camera_world_vector_round(c, camera_screen_to_world_vector(c, v));

    if (!QuadContains(&g->quad, h)) {
        return;
    }

    if (WorldGridGet(g, h)) {
        WorldGridClear(g, h);
    } else {
        WorldGridSet(g, h, style);

        ++style;
        if (style > WorldManagerHexFillMax) {
            style = WorldManagerHexFillMin;
        }
    }

    Quad quad = {h, h};
    texture_update_in_quad(&ui->world_grid_styles_texture, g, &quad);
}
*/
