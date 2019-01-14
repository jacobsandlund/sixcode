#ifndef __MESH_H__
#define __MESH_H__

#include "sixcode.h"
#include "point.h"
#include "layout.h"
#include "grid.h"
#include "hex.h"

typedef struct {
	Point *points;
	i32 hex_capacity;
	i32 hex_count;
} Mesh;

typedef struct {
	Mesh mesh;
	i32 *hex_style_indices;
	i32 *hex_count_for_style;
	i32 style_count;
} StyledMesh;

typedef i32 (*StyledMeshStyle)(void *context, Hex h, void *datum);

Mesh *mesh_create(i32 hex_capacity);
void mesh_destroy(Mesh *m);
void mesh_clear(Mesh *m);
void mesh_expand_hex_capacity(Mesh *m, i32 need_capacity);
void mesh_add_hexes(Mesh *m, Layout *l, Grid *g);

StyledMesh *styled_mesh_create(i32 hex_capacity, i32 style_count);
void styled_mesh_destroy(StyledMesh *sm);
void styled_mesh_clear(StyledMesh *sm);
void styled_mesh_expand_hex_capacity(StyledMesh *sm, i32 need_capacity);
void styled_mesh_add_hexes(StyledMesh *sm, Layout *l, Grid *g, void *style_context, StyledMeshStyle style_fn);

#endif // __MESH_H__
