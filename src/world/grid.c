#include "world/grid.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define WorldGridHalfSize 2048

void world_grid_init(WorldGrid *wg)
{
    wg->quad = (Quad) {
        {-WorldGridHalfSize + 1, -WorldGridHalfSize + 1},
        {WorldGridHalfSize - 2, WorldGridHalfSize - 2},
    };
    wg->styles_quad = (Quad) {
        {-WorldGridHalfSize, -WorldGridHalfSize},
        {WorldGridHalfSize - 1, WorldGridHalfSize - 1},
    };
    quad_to_size_quad(&wg->size_quad, &wg->styles_quad);
    i64 capacity = size_quad_capacity(&wg->size_quad);
    wg->styles = calloc(capacity, sizeof *wg->styles);
}

void world_grid_destroy(WorldGrid *wg)
{
    free(wg->styles);
}

static i64 world_grid_index(WorldGrid *wg, int2 h)
{
    assert(quad_contains(&wg->quad, h));
    i64 diff_min_x = h.x - wg->size_quad.min.x;
    i64 diff_min_y = h.y - wg->size_quad.min.y;
    return diff_min_x + diff_min_y * wg->size_quad.size.x;
}

u8 world_grid_get(WorldGrid *wg, int2 h)
{
    assert(quad_contains(&wg->quad, h));
    return wg->styles[world_grid_index(wg, h)];
}

void world_grid_set(WorldGrid *wg, int2 h, u8 style)
{
    assert(quad_contains(&wg->quad, h));
    i64 i = world_grid_index(wg, h);
    wg->styles[i] = style;
}

void world_grid_clear(WorldGrid *wg, int2 h)
{
    assert(quad_contains(&wg->quad, h));
    i64 i = world_grid_index(wg, h);
    wg->styles[i] = 0;
}
