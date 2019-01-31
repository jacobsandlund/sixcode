#ifndef __MESH_H__
#define __MESH_H__

#include "matrix.h"
#include "sixcode.h"

typedef struct {
	f32 x;
	f32 y;
	i8 c;
	i8 r;
	i8 c2;
	i8 r2;
} MeshVertex;

typedef struct {
	MeshVertex *vertices;
	u16 *fill_indices;
	u16 *stroke_indices;
	i32 vertices_length;
	i32 fill_indices_length;
	i32 stroke_indices_length;
} Mesh;

void mesh_initialize(Mesh *m, i32 num_columns, i32 num_rows);
void mesh_terminate(Mesh *m);

#endif // __MESH_H__
