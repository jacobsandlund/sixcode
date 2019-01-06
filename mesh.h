#ifndef __MESH_H__
#define __MESH_H__

#include "sixcode.h"
#include "point.h"
#include "layout.h"
#include "group.h"

typedef struct {
	Point *points;
	u32 point_count;
	u32 point_capacity;
} Mesh;

Mesh *mesh_create(u32 point_capacity);
u32 mesh_generate_hexes(Mesh *m, Layout *l, Group *g);

#endif // __MESH_H__
