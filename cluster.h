#ifndef __CLUSTER_H__
#define __CLUSTER_H__

#include "sixcode.h"
#include "hex.h"
#include "area.h"

typedef struct {
	Area area;
	Area capacity_area;
	u32 data_count;
	void **data;
} Cluster;


// Return:
//   -1: a < b
//    0: a == b
//    1: a > b
typedef i8 (*ClusterCompare)(void *a, void *b);

typedef void (*ClusterEach)(void *context, Hex h, void *datum);

Cluster *cluster_create(Area capacity_area);
void cluster_destroy(Cluster *c);
void *cluster_get(Cluster *c, Hex h);
void cluster_set(Cluster *c, Hex h, void *datum);

// Returns 1 if item existed and was removed, 0 if not
u8 cluster_remove(Cluster *c, Hex h);

void cluster_move(Cluster *c, Hex move_by);

#endif // __CLUSTER_H__
