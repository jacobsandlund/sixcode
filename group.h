#ifndef __GROUP_H__
#define __GROUP_H__

#include "sixcode.h"
#include "cluster.h"

typedef struct {
	Cluster **clusters;
	u32 count;
	u32 capacity;
} Group;

typedef void (*GroupEach)(void *context, Cluster *c);

Group *group_create(u32 capacity);
void group_destroy(Group *g);
void group_add_cluster(Group *g, Cluster *c);
Cluster *group_nearest_cluster(Group *g, Hex h);
void group_each_cluster(Group *g, void *context, GroupEach each_fn);

#endif // __GROUP_H__
