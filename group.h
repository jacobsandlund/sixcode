#ifndef __GROUP_H__
#define __GROUP_H__

#include "sixcode.h"
#include "hex.h"

typedef struct {
	Hex min;
	Hex max;
	Hex capacity_min;
	Hex capacity_max;
	u32 r_spacing;
	u32 capacity;
	u32 count;
	void **data;
} Group;


// Return:
//   -1: a < b
//    0: a == b
//    1: a > b
typedef i8 (*GroupCompare)(void *a, void *b);

typedef void (*GroupEach)(Hex h, void *datum);

Group *group_create(Hex capacity_min, Hex capacity_max);
void group_destroy(Group *g);
void *group_get(Group *g, Hex h);
u32 group_distance(Group *g, Hex h);

// Returns new_capacity - old_capacity
u32 group_set(Group *g, Hex h, void *datum);

// Returns 1 if item existed and was removed, 0 if not
u8 group_remove(Group *g, Hex h);

// Returns 1 if equal, 0 if not
u8 group_equal(Group *a, Group *b, GroupCompare compare_fn);

void group_each(Group *a, GroupEach each_fn);

#endif // __GROUP_H__
