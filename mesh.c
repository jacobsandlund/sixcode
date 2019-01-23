#include <math.h>
#include <stdlib.h>
#include "mesh.h"

#define MESH_FILL_INDICES_PER_HEX 12
#define MESH_VERTICES_PER_HEX 6

const mat2 MESH_DOUBLED_HEX_TO_POINT_MATRIX = {{
	{0.8660254037844386,	0.0},	// sqrt(3) / 2.0
	{0.0,			1.5},	// 3.0 / 2.0
}};

static vec2 mesh_hex_corner(i8 corner)
{
	f64 angle = M_PI / 3.0 * (0.5 - corner);
	return (vec2) {cos(angle), sin(angle)};
}

void mesh_initialize(Mesh *m, i32 num_columns, i32 num_rows)
{
	i32 num_hexes = num_columns * num_rows;
	i32 vertices_length = MESH_VERTICES_PER_HEX * num_hexes;
	i32 fill_indices_length = MESH_FILL_INDICES_PER_HEX * num_hexes;

	m->vertices = malloc(vertices_length * sizeof *m->vertices);
	m->fill_indices = malloc(fill_indices_length * sizeof *m->fill_indices);
	m->vertices_length = vertices_length;
	m->fill_indices_length = fill_indices_length;

	vec2 corners[6];
	corners[0] = mesh_hex_corner(1);
	corners[1] = mesh_hex_corner(2);
	corners[2] = mesh_hex_corner(0);
	corners[3] = mesh_hex_corner(3);
	corners[4] = mesh_hex_corner(5);
	corners[5] = mesh_hex_corner(4);

	u16 fill_indices_single[] = {
		0, 1, 2,
		2, 1, 3,
		2, 3, 4,
		4, 3, 5,
	};

	i32 vi = 0;
	i32 fi = 0;

	for (i32 r = 0; r < num_rows; ++r) {
		for (i32 c = 0; c < num_columns; ++c) {
			vec2 doubled_hex = {c * 2 + (r & 1), r};
			vec2 center = mat2_multiply_v(&MESH_DOUBLED_HEX_TO_POINT_MATRIX, doubled_hex);

			for (i32 i = 0; i < MESH_FILL_INDICES_PER_HEX; ++i) {
				m->fill_indices[fi + i] = vi + fill_indices_single[i];
			}

			fi += MESH_FILL_INDICES_PER_HEX;

			for (i32 i = 0; i < MESH_VERTICES_PER_HEX; ++i) {
				MeshVertex *vx = &m->vertices[vi + i];
				vx->x = corners[i].x + center.x;
				vx->y = corners[i].y + center.y;
				vx->c = c;
				vx->r = r;
			}

			vi += MESH_VERTICES_PER_HEX;
		}
	}
}

void mesh_terminate(Mesh *m)
{
	free(m->vertices);
	free(m->fill_indices);
}
