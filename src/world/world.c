#include "world/world.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define WorldHexFillMin 1
#define WorldHexFillMax 15

void world_init(World *w)
{
    world_grid_init(&w->grid);
}

void world_destroy(World *w)
{
    world_grid_destroy(&w->grid);
}

void world_load_random(World *w, i64 count)
{
    srand((unsigned int) time(NULL));
    double size = 4096.0 - 2.0;
    i64 style = 0;

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

        world_grid_set(&w->grid, h, style);
    }
}

/*
void world_toggle_hex_at_point(World *w, vec2 v)
{
    static u8 style = WorldHexFillMin;

    ivec2 h = camera_world_vector_round(c, camera_screen_to_world_vector(c, v));

    if (!quad_contains(&g->quad, h)) {
        return;
    }

    if (world_grid_get(g, h)) {
        world_grid_clear(g, h);
    } else {
        world_grid_set(g, h, style);

        ++style;
        if (style > WorldHexFillMax) {
            style = WorldHexFillMin;
        }
    }

    Quad quad = {h, h};
    texture_update_in_quad(&ui->world_grid_styles_texture, g, &quad);
}
*/
