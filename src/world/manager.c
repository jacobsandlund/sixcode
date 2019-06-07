#include "world/manager.h"
#include <math.h>
#include <stdlib.h>
#include "os/clock.h"

#define WorldManagerHexFillMin 1
#define WorldManagerHexFillMax 15

WorldManager gWorldManager;

void world_manager_init(void)
{
    world_grid_init(&gWorldManager.grid);
}

void world_manager_destroy(void)
{
    world_grid_destroy(&gWorldManager.grid);
}

void world_manager_load_random(i64 count)
{
    srand((unsigned int) os_clock_time());
    double size = 4096.0 - 2.0;
    i64 style = 0;

    WorldGrid *g = &gWorldManager.grid;

    for (i64 i = 0; i < count; i++) {
        double rand1 = (double) rand() / (double) RAND_MAX;
        double rand2 = (double) rand() / (double) RAND_MAX;
        int2 h = {
            floor(rand1 * size) - 4096/2 + 1,
            floor(rand2 * size) - 4096/2 + 1,
        };

        style++;
        if (style == 16) {
            style = 1;
        }

        world_grid_set(g, h, style);
    }
}

/*
void world_manager_update(void)
{
    static u8 style = WorldManagerHexFillMin;

    ivec2 h = camera_world_vector_round(c, camera_screen_to_world_vector(c, v));

    if (!quad_contains(&g->quad, h)) {
        return;
    }

    if (world_grid_get(g, h)) {
        world_grid_clear(g, h);
    } else {
        world_grid_set(g, h, style);

        ++style;
        if (style > WorldManagerHexFillMax) {
            style = WorldManagerHexFillMin;
        }
    }

    Quad quad = {h, h};
    texture_update_in_quad(&ui->world_grid_styles_texture, g, &quad);
}
*/
