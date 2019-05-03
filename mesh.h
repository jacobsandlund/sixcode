#ifndef __MESH_H__
#define __MESH_H__

#include "sixcode.h"

typedef struct {
	float x;
	float y;
	i8 hx;
	i8 hy;
} FillMeshVertex;

typedef struct {
	vec2 positionOffset;
	ivec2 gridPositionOffset;
} InstanceMeshVertex;

typedef struct {
	FillMeshVertex *vertices;
	u16 *indices;
	int vertices_length;
	int indices_length;
	int size_x;
	int size_y;
} FillMesh;

typedef struct {
	InstanceMeshVertex *vertices;
	int vertices_length;
	int vertices_capacity;
} InstanceMesh;

void fill_mesh_initialize(FillMesh *m, int size_x, int size_y);
void fill_mesh_terminate(FillMesh *m);
void instance_mesh_initialize(InstanceMesh *m, int length);
void instance_mesh_resize(InstanceMesh *m, int length);
void instance_mesh_terminate(InstanceMesh *m);

#endif // __MESH_H__
