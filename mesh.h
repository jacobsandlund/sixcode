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
} FillMeshVertex;

typedef struct {
	f32 x;
	f32 y;
	i8 c;
	i8 r;
	i8 c2;
	i8 r2;
} StrokeMeshVertex;

typedef struct {
	FillMeshVertex *vertices;
	u16 *indices;
	i32 vertices_length;
	i32 indices_length;
	i32 size_c;
	i32 size_r;
} FillMesh;

typedef struct {
	StrokeMeshVertex *vertices;
	i32 vertices_length;
	i32 size_c;
	i32 size_r;
} StrokeMesh;

void fill_mesh_initialize(FillMesh *m, i32 size_c, i32 size_r);
void fill_mesh_terminate(FillMesh *m);
void stroke_mesh_initialize(StrokeMesh *m, i32 size_c, i32 size_r);
void stroke_mesh_terminate(StrokeMesh *m);

#endif // __MESH_H__
