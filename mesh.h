#ifndef __MESH_H__
#define __MESH_H__

#include "sixcode.h"
#include "point.h"
#include "layout.h"
#include "grid.h"
#include "hex.h"

typedef struct {
	Point *points;
	u32 hex_count;
	u32 hex_capacity;
} Mesh;

Mesh *mesh_create(u32 hex_capacity);
void mesh_destroy(Mesh *m);
u32 mesh_generate_hexes(Mesh *m, Layout *l, Grid *g);

#endif // __MESH_H__
