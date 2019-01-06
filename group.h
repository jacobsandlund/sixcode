#ifndef __GROUP_H__
#define __GROUP_H__

#include "sixcode.h"
#include "cluster.h"

typedef struct {
	Cluster **clusters;
	u32 cluster_count;
	u32 cluster_capacity;
} Group;

typedef void (*GroupEach)(void *context, Hex h, void *datum);

Group *group_create(u32 cluster_capacity);
void group_destroy(Group *g);
void *group_get(Group *g, Hex h);
void group_set(Group *g, Hex h, void *datum);

// Returns 1 if item existed and was removed, 0 if not
u8 group_remove(Group *g, Hex h);

u32 group_hex_count(Group *g);
void group_each(Group *g, void *context, GroupEach each_fn);

#endif // __GROUP_H__
