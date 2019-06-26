#include "World/Grid.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void WorldGridInit(WorldGrid *wg, i32 size)
{
    i32 half_size = size / 2;
    wg->quad = (Quad){
        { -half_size, -half_size },
        { half_size - 1, half_size - 1 },
    };
    quad_to_size_quad(&wg->size_quad, &wg->quad);
    i64 capacity = SizeQuadCapacity(&wg->size_quad);
    wg->styles = calloc(capacity, sizeof *wg->styles);
}

void WorldGridDestroy(WorldGrid *wg)
{
    free(wg->styles);
}

static i64 WorldGridIndex(WorldGrid *wg, int2 h)
{
    assert(QuadContains(&wg->quad, h));
    i64 diff_min_x = h.x - wg->size_quad.min.x;
    i64 diff_min_y = h.y - wg->size_quad.min.y;
    return diff_min_x + diff_min_y * wg->size_quad.size.x;
}

u8 WorldGridGet(WorldGrid *wg, int2 h)
{
    assert(QuadContains(&wg->quad, h));
    return wg->styles[WorldGridIndex(wg, h)];
}

void WorldGridSet(WorldGrid *wg, int2 h, u8 style)
{
    assert(QuadContains(&wg->quad, h));
    i64 i = WorldGridIndex(wg, h);
    wg->styles[i] = style;
}

void WorldGridClear(WorldGrid *wg, int2 h)
{
    assert(QuadContains(&wg->quad, h));
    i64 i = WorldGridIndex(wg, h);
    wg->styles[i] = 0;
}
