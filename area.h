#ifndef __AREA_H__
#define __AREA_H__

#include "sixcode.h"
#include "group.h"

typedef struct {
	Group **groups;
	u32 count;
	u32 capacity;
} Area;

typedef void (*AreaEach)(void *context, Group *g);

Area *area_create(u32 capacity);
void area_destroy(Area *a);
void area_add_group(Area *a, Group *g);
Group *area_nearest_group(Area *a, Hex h);
void area_each_group(Area *a, void *context, AreaEach each_fn);

#endif // __AREA_H__
