#ifndef Mesh_h
#define Mesh_h

#include "spacetime.h"
#include "layout.h"

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

void fill_mesh_init(FillMesh *m, LayoutType layout_type, i64 size_x, i64 size_y);
void fill_mesh_destroy(FillMesh *m);
void instance_mesh_init(InstanceMesh *m, i64 length);
void instance_mesh_destroy(InstanceMesh *m);
void instance_mesh_resize(InstanceMesh *m, i64 length);

#endif // Mesh_h
