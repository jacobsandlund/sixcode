#include <stdlib.h>
#include "mesh.h"

Mesh *mesh_create(u32 point_capacity)
{
	Mesh *m = malloc(sizeof *m);

	m->points = malloc(point_capacity * sizeof *m->points);
	m->point_count = 0;
	m->point_capacity = point_capacity;

	return m;
}

typedef struct {
	Mesh *m;
	Layout *l;
} MeshEachHexContext;

static void mesh_generate_each_hex(void *context, Hex h, void *datum) {
	MeshEachHexContext *c = (MeshEachHexContext *) context;
	Mesh *m = c->m;
	Layout *l = c->l;

	(void) datum;

	layout_hex_corners(&m->points[m->point_count], l, h);

	m->point_count += 6;
}

u32 mesh_generate_hexes(Mesh *m, Layout *l, Grid *g)
{
	MeshEachHexContext c = {.m = m, .l = l};
	u32 point_count = g->data_count * 6;

	if (point_count > m->point_count) {
		free(m->points);
		m->points = malloc(point_count * sizeof *m->points);
		m->point_capacity = point_count;
	}

	m->point_count = 0;

	grid_each(g, &c, mesh_generate_each_hex);

	return point_count;
}
