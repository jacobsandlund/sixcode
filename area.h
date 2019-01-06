#ifndef __AREA_H__
#define __AREA_H__

#include "sixcode.h"
#include "hex.h"

typedef struct {
	Hex min;
	Hex max;
} Area;

extern const Area AREA_ZERO;
extern const Area AREA_EMPTY;

u8 area_contains(Area a, Hex h);
u8 area_on_edge(Area a, Hex h);
u8 area_empty(Area a);
u32 area_distance(Area a, Hex h);
i32 area_index(Area a, Hex h);
i32 area_capacity(Area a);
Area area_expand(Area a, Hex include_hex, f64 growth_factor);
Area area_move(Area a, Hex move_by);
u8 area_equal(Area a, Area b);

#endif // __AREA_H__
