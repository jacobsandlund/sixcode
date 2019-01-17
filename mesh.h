#ifndef __MESH_H__
#define __MESH_H__

#include "grid.h"
#include "hex.h"
#include "layout.h"
#include "point.h"
#include "sixcode.h"

typedef struct {
	Point *points;
	i32 point_capacity;
	i32 hex_count;
	i32 points_per_hex;
} Mesh;

typedef struct {
	Mesh mesh;
	i32 *hex_style_indices;
	i32 *hex_count_for_style;
	i32 style_count;
} StyledMesh;

typedef i32 (*StyledMeshStyle)(void *context, Hex h, void *data);

Mesh *mesh_create();
void mesh_destroy(Mesh *m);
void mesh_free_capacity(Mesh *m);
void mesh_clear(Mesh *m);
void mesh_ensure_capacity(Mesh *m, i32 need_capacity, i32 points_per_hex);
void mesh_add_hexes(Mesh *m, Layout *l, Grid *g);
void mesh_add_points_at_hexes(Mesh *m, Layout *l, Grid *g);

StyledMesh *styled_mesh_create(i32 style_count);
void styled_mesh_destroy(StyledMesh *sm);
void styled_mesh_free_capacity(StyledMesh *sm);
void styled_mesh_clear(StyledMesh *sm);
void styled_mesh_ensure_capacity(StyledMesh *sm, i32 need_capacity, i32 points_per_hex);
void styled_mesh_add_hexes(StyledMesh *sm, Layout *l, Grid *g, void *style_context, StyledMeshStyle style_fn);
void styled_mesh_add_points_at_hexes(StyledMesh *sm, Layout *l, Grid *g, void *style_context, StyledMeshStyle style_fn);

#endif // __MESH_H__
