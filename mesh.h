#ifndef __MESH_H__
#define __MESH_H__

#include "sixcode.h"
#include "hex.h"

typedef struct {
	f32 x;
	f32 y;
	i8 c;
	i8 r;
} FillMeshVertex;

typedef struct {
	vec2 positionOffset;
	Hex gridPositionOffset;
} InstanceMeshVertex;

typedef struct {
	FillMeshVertex *vertices;
	u16 *indices;
	i32 vertices_length;
	i32 indices_length;
	i32 size_c;
	i32 size_r;
} FillMesh;

typedef struct {
	InstanceMeshVertex *vertices;
	i32 vertices_length;
	i32 vertices_capacity;
} InstanceMesh;

void fill_mesh_initialize(FillMesh *m, i32 size_c, i32 size_r);
void fill_mesh_terminate(FillMesh *m);
void instance_mesh_initialize(InstanceMesh *m, i32 length);
void instance_mesh_resize(InstanceMesh *m, i32 length);
void instance_mesh_terminate(InstanceMesh *m);

#endif // __MESH_H__
