#ifndef __MESH_H__
#define __MESH_H__

#include "sixcode.h"
#include "point.h"
#include "layout.h"
#include "grid.h"
#include "hex.h"

typedef struct {
	Point *points;
	i32 hex_count;
	i32 hex_capacity;
} Mesh;

Mesh *mesh_create(i32 hex_capacity);
void mesh_destroy(Mesh *m);
i32 mesh_generate_hexes(Mesh *m, Layout *l, Grid *g);

#endif // __MESH_H__
