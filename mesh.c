#include <math.h>
#include <stdlib.h>
#include "mesh.h"

#define FILL_MESH_VERTICES_PER_HEX 6
#define FILL_MESH_INDICES_PER_HEX 12
#define STROKE_MESH_VERTICES_PER_HEX 6

static const mat2 MESH_HEX_TO_POINT = {{
	{0.8660254037844386,	0.0},	// sqrt(3) / 2.0
	{0.0,			-1.5},	// 3.0 / 2.0
}};

static vec2 mesh_hex_corner(i8 corner)
{
	f64 angle = M_PI / 3.0 * (0.5 + corner);
	return (vec2) {cos(angle), sin(angle)};
}

void fill_mesh_initialize(FillMesh *m, i32 size_c, i32 size_r)
{
	i32 num_hexes = size_c * size_r;
	m->vertices_length = FILL_MESH_VERTICES_PER_HEX * num_hexes;
	m->indices_length = FILL_MESH_INDICES_PER_HEX * num_hexes;
	m->size_c = size_c;
	m->size_r = size_r;

	m->vertices = malloc(m->vertices_length * sizeof *m->vertices);
	m->indices = malloc(m->indices_length * sizeof *m->indices);

	vec2 corners[] = {
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
		5, 3, 4,
	};

	i32 vi = 0;
	i32 ii = 0;

	for (i32 r = 0; r < size_r; ++r) {
		for (i32 c = 0; c < size_c; ++c) {
			vec2 h = {(c << 1) + (r & 1), r};
			vec2 center = mat2_multiply_v(&MESH_HEX_TO_POINT, h);

			for (i32 i = 0; i < FILL_MESH_INDICES_PER_HEX; ++i) {
				m->indices[ii + i] = vi + indices_single[i];
			}

			ii += FILL_MESH_INDICES_PER_HEX;

			for (i32 i = 0; i < FILL_MESH_VERTICES_PER_HEX; ++i) {
				FillMeshVertex *vx = &m->vertices[vi + i];
				vx->x = corners[i].x + center.x;
				vx->y = corners[i].y + center.y;
				vx->c = c;
				vx->r = r;
			}

			vi += FILL_MESH_VERTICES_PER_HEX;
		}
	}
}

void fill_mesh_terminate(FillMesh *m)
{
	free(m->vertices);
	free(m->indices);
}

void stroke_mesh_initialize(StrokeMesh *m, i32 size_c, i32 size_r)
{
	i32 num_hexes = size_c * size_r;
	m->vertices_length = STROKE_MESH_VERTICES_PER_HEX * num_hexes;
	m->size_c = size_c;
	m->size_r = size_r;

	m->vertices = malloc(m->vertices_length * sizeof *m->vertices);

	vec2 corners[] = {
		mesh_hex_corner(0),
		mesh_hex_corner(1),
		mesh_hex_corner(1),
		mesh_hex_corner(2),
		mesh_hex_corner(2),
		mesh_hex_corner(3),
	};

	Hex neighbor_offsets[] = {
		{+1, -1},
		{+1, -1},
		{-1, -1},
		{-1, -1},
		{-2, 0},
		{-2, 0},
	};

	i32 vi = 0;

	for (i32 r = 0; r < size_r; ++r) {
		for (i32 c = 0; c < size_c; ++c) {
			vec2 h = {(c << 1) + (r & 1), r};
			vec2 center = mat2_multiply_v(&MESH_HEX_TO_POINT, h);

			for (i32 i = 0; i < STROKE_MESH_VERTICES_PER_HEX; ++i) {
				StrokeMeshVertex *vx = &m->vertices[vi + i];
				vx->x = corners[i].x + center.x;
				vx->y = corners[i].y + center.y;
				vx->c = c;
				vx->r = r;

				Hex offset = neighbor_offsets[i];
				Hex h2 = {h.x + offset.c, h.y + offset.r};
				vx->c2 = h2.c >> 1;
				vx->r2 = h2.r;
			}

			vi += STROKE_MESH_VERTICES_PER_HEX;
		}
	}
}

void stroke_mesh_terminate(StrokeMesh *m)
{
	free(m->vertices);
}
