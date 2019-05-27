#ifndef Mesh_h
#define Mesh_h

#include "spacetime.h"
#include "resource-manager.h"

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

i64 fill_mesh_vertices_hex_length(i64 size);
void fill_mesh_vertices_hex_set(void *contents, uintptr_t content_init_options);
void fill_mesh_indices_hex_set(void *contents, uintptr_t content_init_options);
void fill_mesh_vertices_rect_set(void *contents, uintptr_t content_init_options);
void fill_mesh_indices_rect_set(void *contents, uintptr_t content_init_options);

#endif // Mesh_h
