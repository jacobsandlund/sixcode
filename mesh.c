#include <stdlib.h>
#include "mesh.h"

Mesh *mesh_create(u32 hex_capacity)
{
	Mesh *m = malloc(sizeof *m);

	m->points = malloc(hex_capacity * 6 * sizeof *m->points);
	m->hex_count = 0;
	m->hex_capacity = hex_capacity;

	return m;
}

void mesh_destroy(Mesh *m)
{
	free(m->points);
	free(m);
}

typedef struct {
	Mesh *m;
	Layout *l;
} MeshEachContext;

static void mesh_generate_each(void *context, Hex h, void *datum) {
	MeshEachContext *c = (MeshEachContext *) context;
	Mesh *m = c->m;
	Layout *l = c->l;

	(void) datum;

	Point *corners = &m->points[m->hex_count * 6];
	layout_hex_corners(corners, l, h);

	++m->hex_count;
}

u32 mesh_generate_hexes(Mesh *m, Layout *l, Grid *g)
{
	MeshEachContext c = {.m = m, .l = l};

	if (g->set_count > m->hex_capacity) {
		free(m->points);
		m->points = malloc(g->set_count * 6 * sizeof *m->points);
		m->hex_capacity = g->set_count;
	}

	m->hex_count = 0;

	grid_each(g, &c, mesh_generate_each);

	return m->hex_count;
}
