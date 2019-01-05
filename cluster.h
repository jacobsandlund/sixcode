#ifndef __CLUSTER_H__
#define __CLUSTER_H__

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
} Cluster;


// Return:
//   -1: a < b
//    0: a == b
//    1: a > b
typedef i8 (*ClusterCompare)(void *a, void *b);

typedef void (*ClusterEach)(void *context, Hex h, void *datum);

Cluster *cluster_create(Hex capacity_min, Hex capacity_max);
void cluster_destroy(Cluster *c);
void *cluster_get(Cluster *c, Hex h);
u32 cluster_distance(Cluster *c, Hex h);

// Returns new_capacity - old_capacity
u32 cluster_set(Cluster *c, Hex h, void *datum);

// Returns 1 if item existed and was removed, 0 if not
u8 cluster_remove(Cluster *c, Hex h);

void cluster_move(Cluster *c, Hex move_by);

// Returns 1 if equal, 0 if not
u8 cluster_equal(Cluster *a, Cluster *b, ClusterCompare compare_fn);

void cluster_each(Cluster *c, void *context, ClusterEach each_fn);

#endif // __CLUSTER_H__
