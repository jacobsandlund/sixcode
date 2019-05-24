#include "mesh.h"
#include <math.h>
#include <stdlib.h>

#define FillMeshVerticesPerHex 6
#define FillMeshIndicesPerHex 12
#define FillMeshVerticesPerRect 4
#define FillMeshIndicesPerRect 6
#define FillMeshFraction 0.95

static float2 mesh_hex_corner(i64 corner)
{
	double angle = -M_PI / 3.0 * (0.5 + corner);
	Layout l;
	layout_type(&l, LayoutTypeHex);

	return (float2) {
		cos(angle) * FillMeshFraction / l.scale.x,
		sin(angle) * FillMeshFraction / l.scale.y,
	};
}

void fill_mesh_init_hex(FillMesh *m, i64 size)
{
	i64 num_hexes = size * size;
	m->vertices_length = FillMeshVerticesPerHex * num_hexes;
	m->indices_length = FillMeshIndicesPerHex * num_hexes;
	m->size = size;

	m->vertices = malloc(m->vertices_length * sizeof *m->vertices);
	m->indices = malloc(m->indices_length * sizeof *m->indices);

	float2 corners[] = {
		mesh_hex_corner(0),
		mesh_hex_corner(1),
		mesh_hex_corner(2),
		mesh_hex_corner(3),
		mesh_hex_corner(4),
		mesh_hex_corner(5),
	};

	u16 indices_single[] = {
		1, 2, 0,
		0, 2, 3,
		0, 3, 5,
		3, 4, 5,
	};

	int2 h;
	i64 vi = 0;
	i64 ii = 0;

	for (h.y = 0; h.y < size_y; h.y++) {
		for (h.x = 0; h.x < size_x; h.x++) {
			float2 center = float2_from_int2(h);
			center.x += 0.5 * (h.y & 1);

			for (i64 i = 0; i < FillMeshIndicesPerHex; i++) {
				m->indices[ii + i] = vi + indices_single[i];
			}

			ii += FillMeshIndicesPerHex;

			for (i64 i = 0; i < FillMeshVerticesPerHex; i++) {
				FillMeshVertex *vx = &m->vertices[vi + i];
				vx->x = corners[i].x + center.x;
				vx->y = corners[i].y + center.y;
				vx->hx = h.x;
				vx->hy = h.y;
			}

			vi += FillMeshVerticesPerHex;
		}
	}
}

void fill_mesh_init_rect(FillMesh *m, i64 size)
{
	i64 num_rects = size * size;
	m->vertices_length = FillMeshVerticesPerRect * num_rects;
	m->indices_length = FillMeshIndicesPerRect * num_rects;
	m->size = size;

	m->vertices = malloc(m->vertices_length * sizeof *m->vertices);
	m->indices = malloc(m->indices_length * sizeof *m->indices);

	float2 size = {
		0.5 * FillMeshFraction,
		0.5 * FillMeshFraction,
	};

	float2 corners[] = {
		{size.x, -size.y},
		{-size.x, -size.y},
		{-size.x, size.y},
		{size.x, size.y},
	};

	u16 indices_single[] = {
		0, 1, 2,
		0, 2, 3,
	};

	int2 h;
	i64 vi = 0;
	i64 ii = 0;

	for (h.y = 0; h.y < size_y; h.y++) {
		for (h.x = 0; h.x < size_x; h.x++) {
			float2 center = float2_from_int2(h);

			for (i64 i = 0; i < FillMeshIndicesPerRect; i++) {
				m->indices[ii + i] = vi + indices_single[i];
			}

			ii += FillMeshIndicesPerRect;

			for (i64 i = 0; i < FillMeshVerticesPerRect; i++) {
				FillMeshVertex *vx = &m->vertices[vi + i];
				vx->x = corners[i].x + center.x;
				vx->y = corners[i].y + center.y;
				vx->hx = h.x;
				vx->hy = h.y;
			}

			vi += FillMeshVerticesPerRect;
		}
	}
}

void fill_mesh_init(void *pointer, uintptr_t raw_options)
{
	FillMesh *m = (FillMesh *) pointer;
	FillMeshOptions *options = (FillMeshOptions *) raw_options;

	switch (options->layout_type) {
	case LayoutTypeHex:
		fill_mesh_init_hex(m, options->size);
		break;
	case LayoutTypeRect:
		fill_mesh_init_rect(m, options->size);
		break;
	}
}

void fill_mesh_destroy(void *pointer)
{
	FillMesh *m = (FillMesh *) pointer;
	free(m->vertices);
	free(m->indices);
}

void instance_mesh_init(InstanceMesh *m, i64 capacity)
{
	m->vertices = malloc(capacity * sizeof *m->vertices);
	m->vertices_length = 0;
	m->vertices_capacity = capacity;
}

void instance_mesh_destroy(InstanceMesh *m)
{
	free(m->vertices);
}

const ResourceLoader FillMeshResourceLoader = {
	.type = ResourceTypeFillMesh,
	.size = fill_mesh_size,
	.init = fill_mesh_init,
	.destroy = fill_mesh_destroy,
};

