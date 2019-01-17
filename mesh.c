#include <stdlib.h>
#include "mesh.h"

#define MESH_HEX_POINT_SCALE_FACTOR 0.8660254037844386

typedef struct {
	Mesh *m;
	Layout *l;
} MeshEachContext;

typedef struct {
	StyledMesh *sm;
	Layout *l;
	void *style_context;
	StyledMeshStyle style_fn;
} StyledMeshEachContext;

static void mesh_init(Mesh *m)
{
	m->points = NULL;
	m->point_capacity = 0;
	m->hex_count = 0;
	m->points_per_hex = 1;
}

Mesh *mesh_create()
{
	Mesh *m = malloc(sizeof *m);

	mesh_init(m);

	return m;
}

void mesh_free_capacity(Mesh *m)
{
	free(m->points);
	mesh_init(m);
}

void mesh_clear(Mesh *m)
{
	m->hex_count = 0;
}

void mesh_destroy(Mesh *m)
{
	free(m->points);
	free(m);
}

void mesh_ensure_capacity(Mesh *m, i32 need_capacity, i32 points_per_hex)
{
	i32 need_point_capacity = need_capacity * points_per_hex;

	if (points_per_hex != m->points_per_hex) {
		m->hex_count = 0;
	}
	i32 point_count = m->hex_count * m->points_per_hex;
	m->points_per_hex = points_per_hex;

	if (m->point_capacity >= need_point_capacity) {
		return;
	}

	i32 point_capacity = m->point_capacity || 1;

	while (point_capacity < need_point_capacity) {
		point_capacity *= 2;
	}

	Point *points = malloc(point_capacity * sizeof *m->points);

	for (i32 i = 0; i < point_count; ++i) {
		points[i] = m->points[i];
	}

	if (m->points != NULL) {
		free(m->points);
	}

	m->points = points;
	m->point_capacity = point_capacity;
}

static void mesh_generate_hex(Mesh *m, Layout *l, Hex h)
{
	Point *corners = &m->points[m->hex_count * 6];
	layout_hex_corners(corners, l, h);
	++m->hex_count;
}

static void mesh_add_hexes_each_fn(void *context, Hex h, void *data)
{
	MeshEachContext *c = context;
	(void) data;

	mesh_generate_hex(c->m, c->l, h);
}

void mesh_add_hexes(Mesh *m, Layout *l, Grid *g)
{
	MeshEachContext c = {.m = m, .l = l};

	mesh_ensure_capacity(m, g->set_count + m->hex_count, 6);
	grid_each(g, g->quad, &c, mesh_add_hexes_each_fn);
}

static void mesh_generate_point_at_hex(Mesh *m, Layout *l, Hex h)
{
	f64 scale_factor = l->scale * MESH_HEX_POINT_SCALE_FACTOR;
	Point p = layout_hex_to_point(l, h);
	p.x -= scale_factor;
	p.y -= scale_factor;
	m->points[m->hex_count] = p;
	++m->hex_count;
}

static void mesh_add_points_at_hexes_each_fn(void *context, Hex h, void *data)
{
	MeshEachContext *c = context;
	(void) data;

	mesh_generate_point_at_hex(c->m, c->l, h);
}

void mesh_add_points_at_hexes(Mesh *m, Layout *l, Grid *g)
{
	MeshEachContext c = {.m = m, .l = l};

	mesh_ensure_capacity(m, g->set_count + m->hex_count, 1);
	grid_each(g, g->quad, &c, mesh_add_points_at_hexes_each_fn);
}

StyledMesh *styled_mesh_create(i32 style_count)
{
	StyledMesh *sm = malloc(sizeof *sm);

	mesh_init((Mesh *) sm);

	sm->hex_style_indices = NULL;
	sm->hex_count_for_style = malloc(style_count * sizeof *sm->hex_count_for_style);
	sm->style_count = style_count;

	styled_mesh_clear(sm);

	return sm;
}

void styled_mesh_destroy(StyledMesh *sm)
{
	free(sm->hex_style_indices);
	free(sm->hex_count_for_style);
	mesh_destroy((Mesh *) sm);
}

void styled_mesh_free_capacity(StyledMesh *sm)
{
	mesh_free_capacity(&sm->mesh);
	free(sm->hex_style_indices);
	sm->hex_style_indices = NULL;
}

void styled_mesh_clear(StyledMesh *sm)
{
	i32 style_count = sm->style_count;

	mesh_clear(&sm->mesh);

	for (i32 i = 0; i < style_count; ++i) {
		sm->hex_count_for_style[i] = 0;
	}
}

void styled_mesh_ensure_capacity(StyledMesh *sm, i32 need_capacity, i32 points_per_hex)
{
	Mesh *m = &sm->mesh;

	i32 old_point_capacity = m->point_capacity;
	mesh_ensure_capacity(m, need_capacity, points_per_hex);

	if (m->point_capacity != old_point_capacity) {
		i32 hex_count = m->hex_count;
		i32 hex_capacity = m->point_capacity / points_per_hex;
		i32 *hex_style_indices = malloc(hex_capacity * sizeof *sm->hex_style_indices);

		for (i32 i = 0; i < hex_count; ++i) {
			hex_style_indices[i] = sm->hex_style_indices[i];
		}

		if (sm->hex_style_indices != NULL) {
			free(sm->hex_style_indices);
		}

		sm->hex_style_indices = hex_style_indices;
	}
}

static void styled_mesh_add_hexes_each_fn(void *context, Hex h, void *data) {
	StyledMeshEachContext *c = context;
	StyledMesh *sm = c->sm;
	Mesh *m = &sm->mesh;

	i32 style = c->style_fn(c->style_context, h, data);

	sm->hex_style_indices[m->hex_count] = style;
	++sm->hex_count_for_style[style];

	mesh_generate_hex(m, c->l, h);
}

void styled_mesh_add_hexes(StyledMesh *sm, Layout *l, Grid *g, void *style_context, StyledMeshStyle style_fn)
{
	StyledMeshEachContext c = {
		.sm = sm,
		.l = l,
		.style_context = style_context,
		.style_fn = style_fn,
	};
	Mesh *m = &sm->mesh;

	styled_mesh_ensure_capacity(sm, g->set_count + m->hex_count, 6);
	grid_each(g, g->quad, &c, styled_mesh_add_hexes_each_fn);
}

static void styled_mesh_add_points_at_hexes_each_fn(void *context, Hex h, void *data) {
	StyledMeshEachContext *c = context;
	StyledMesh *sm = c->sm;
	Mesh *m = &sm->mesh;

	i32 style = c->style_fn(c->style_context, h, data);

	sm->hex_style_indices[m->hex_count] = style;
	++sm->hex_count_for_style[style];

	mesh_generate_point_at_hex(m, c->l, h);
}

void styled_mesh_add_points_at_hexes(StyledMesh *sm, Layout *l, Grid *g, void *style_context, StyledMeshStyle style_fn)
{
	StyledMeshEachContext c = {
		.sm = sm,
		.l = l,
		.style_context = style_context,
		.style_fn = style_fn,
	};
	Mesh *m = &sm->mesh;

	styled_mesh_ensure_capacity(sm, g->set_count + m->hex_count, 1);
	grid_each(g, g->quad, &c, styled_mesh_add_points_at_hexes_each_fn);
}
