#ifndef Mesh_h
#define Mesh_h

#include "spacetime.h"
#include "layout.h"
#include "resource.h"

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
	i64 size;
} FillMesh;

typedef struct {
	InstanceMeshVertex *vertices;
	i64 vertices_length;
	i64 vertices_capacity;
} InstanceMesh;

typedef struct {
	LayoutType layout_type;
	i64 size;
} FillMeshOptions;

extern const ResourceLoader FillMeshResourceLoader;

void instance_mesh_init(InstanceMesh *m, i64 capacity);
void instance_mesh_destroy(InstanceMesh *m);

#endif // Mesh_h
