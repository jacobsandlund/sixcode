#include <stdlib.h>
#include "mesh.h"

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


static void mesh_init(Mesh *m, i32 hex_capacity)
{
	m->points = malloc(hex_capacity * 6 * sizeof *m->points);
	m->hex_capacity = hex_capacity;
	mesh_clear(m);
}

void mesh_clear(Mesh *m)
{
	m->hex_count = 0;
}

Mesh *mesh_create(i32 hex_capacity)
{
	Mesh *m = malloc(sizeof *m);

	mesh_init(m, hex_capacity);

	return m;
}

void mesh_destroy(Mesh *m)
{
	free(m->points);
	free(m);
}

void mesh_expand_hex_capacity(Mesh *m, i32 need_capacity)
{
	Point *points;
	i32 point_count = m->hex_count * 6;
	i32 hex_capacity = m->hex_capacity;

	while (hex_capacity < need_capacity) {
		hex_capacity *= 2;
	}

	points = malloc(hex_capacity * 6 * sizeof *m->points);

	for (i32 i = 0; i < point_count; ++i) {
		points[i] = m->points[i];
	}

	free(m->points);

	m->points = points;
	m->hex_capacity = hex_capacity;
}

static void mesh_generate_hex(Mesh *m, Layout *l, Hex h)
{
	Point *corners = &m->points[m->hex_count * 6];
	layout_hex_corners(corners, l, h);
	++m->hex_count;
}

static void mesh_generate_each(void *context, Hex h, void *datum)
{
	MeshEachContext *c = (MeshEachContext *) context;
	(void) datum;

	mesh_generate_hex(c->m, c->l, h);
}

void mesh_add_hexes(Mesh *m, Layout *l, Grid *g)
{
	MeshEachContext c = {.m = m, .l = l};
	i32 need_capacity = g->set_count + m->hex_count;

	if (need_capacity > m->hex_capacity) {
		mesh_expand_hex_capacity(m, need_capacity);
	}

	grid_each(g, g->quad, &c, mesh_generate_each);
}

StyledMesh *styled_mesh_create(i32 hex_capacity, i32 style_count)
{
	StyledMesh *sm = malloc(sizeof *sm);

	mesh_init((Mesh *) sm, hex_capacity);

	sm->hex_style_indices = malloc(hex_capacity * sizeof *sm->hex_style_indices);
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

void styled_mesh_clear(StyledMesh *sm)
{
	i32 style_count = sm->style_count;

	mesh_clear(&sm->mesh);

	for (i32 i = 0; i < style_count; ++i) {
		sm->hex_count_for_style[i] = 0;
	}
}

void styled_mesh_expand_hex_capacity(StyledMesh *sm, i32 need_capacity)
{
	Mesh *m = &sm->mesh;
	i32 old_hex_capacity = m->hex_capacity;

	mesh_expand_hex_capacity(m, need_capacity);

	if (m->hex_capacity != old_hex_capacity) {
		i32 hex_count = m->hex_count;
		i32 *hex_style_indices = malloc(m->hex_capacity * sizeof *sm->hex_style_indices);

		for (i32 i = 0; i < hex_count; ++i) {
			hex_style_indices[i] = sm->hex_style_indices[i];
		}

		free(sm->hex_style_indices);

		sm->hex_style_indices = hex_style_indices;
	}
}

static void styled_mesh_generate_each(void *context, Hex h, void *datum) {
	StyledMeshEachContext *c = (StyledMeshEachContext *) context;
	StyledMesh *sm = c->sm;
	Mesh *m = &sm->mesh;

	i32 style = c->style_fn(c->style_context, h, datum);

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
	i32 need_capacity = g->set_count + m->hex_count;

	if (need_capacity > m->hex_capacity) {
		styled_mesh_expand_hex_capacity(sm, need_capacity);
	}

	grid_each(g, g->quad, &c, styled_mesh_generate_each);
}
