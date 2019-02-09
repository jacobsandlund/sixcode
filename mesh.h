#ifndef __MESH_H__
#define __MESH_H__

#include "matrix.h"
#include "sixcode.h"
#include "hex.h"

typedef struct {
	f32 x;
	f32 y;
	i8 c;
	i8 r;
	i8 c2;
	i8 r2;
} MeshVertex;

typedef struct {
	MeshVertex *fill_vertices;
	MeshVertex *stroke_vertices;
	u16 *fill_indices;
	i32 fill_vertices_length;
	i32 fill_indices_length;
	i32 stroke_vertices_length;
	i32 size_c;
	i32 size_r;
} Mesh;

void mesh_initialize(Mesh *m, i32 size_c, i32 size_r);
void mesh_terminate(Mesh *m);

#endif // __MESH_H__
