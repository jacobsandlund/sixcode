#ifndef MESH_H
#define MESH_H

#include "spacetime.h"
#include "viewport.h"

typedef struct {
	float x;
	float y;
	i8 hx;
	i8 hy;
} FillMeshVertex;

typedef struct {
	float2 positionOffset;
	int2 gridPositionOffset;
} InstanceMeshVertex;

typedef struct {
	FillMeshVertex *vertices;
	u16 *indices;
	i64 vertices_length;
	i64 indices_length;
	i64 size_x;
	i64 size_y;
} FillMesh;

typedef struct {
	InstanceMeshVertex *vertices;
	i64 vertices_length;
	i64 vertices_capacity;
} InstanceMesh;

void fill_mesh_initialize(FillMesh *m, CameraLayout layout, i64 size_x, i64 size_y);
void fill_mesh_terminate(FillMesh *m);
void instance_mesh_initialize(InstanceMesh *m, i64 length);
void instance_mesh_resize(InstanceMesh *m, i64 length);
void instance_mesh_terminate(InstanceMesh *m);

#endif // MESH_H
