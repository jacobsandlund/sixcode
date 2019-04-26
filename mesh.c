#include <math.h>
#include <stdlib.h>
#include "hex.h"
#include "mesh.h"
#include "space.h"
#include "vector.h"

#define FILL_MESH_VERTICES_PER_HEX 6
#define FILL_MESH_INDICES_PER_HEX 12
#define STROKE_MESH_VERTICES_PER_HEX 6

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
		3, 4, 5,
	};

	Hex h;
	i32 vi = 0;
	i32 ii = 0;

	for (h.r = 0; h.r < size_r; ++h.r) {
		for (h.c = 0; h.c < size_c; ++h.c) {
			vec2 center = space_hex_to_world(
					space_hex_to_vec(
					space_storage_to_hex(h)));

			for (i32 i = 0; i < FILL_MESH_INDICES_PER_HEX; ++i) {
				m->indices[ii + i] = vi + indices_single[i];
			}

			ii += FILL_MESH_INDICES_PER_HEX;

			for (i32 i = 0; i < FILL_MESH_VERTICES_PER_HEX; ++i) {
				FillMeshVertex *vx = &m->vertices[vi + i];
				vx->x = corners[i].x + center.x;
				vx->y = corners[i].y + center.y;
				vx->c = h.c;
				vx->r = h.r;
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

void points_mesh_initialize(PointsMesh *m, i32 size_c, i32 size_r)
{
	m->vertices_length = size_c * size_r;
	m->size_c = size_c;
	m->size_r = size_r;

	m->vertices = malloc(m->vertices_length * sizeof *m->vertices);

	Hex h;
	i32 vi = 0;

	for (h.r = 0; h.r < size_r; ++h.r) {
		for (h.c = 0; h.c < size_c; ++h.c) {
			vec2 center = space_hex_to_world(
					space_hex_to_vec(
					space_storage_to_hex(h)));

			PointsMeshVertex *vx = &m->vertices[vi];
			vx->x = center.x;
			vx->y = center.y;
			vx->c = h.c;
			vx->r = h.r;

			++vi;
		}
	}
}

void points_mesh_terminate(PointsMesh *m)
{
	free(m->vertices);
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

	Hex h;
	i32 vi = 0;

	for (h.r = 0; h.r < size_r; ++h.r) {
		for (h.c = 0; h.c < size_c; ++h.c) {
			Hex hex_space_h = space_storage_to_hex(h);
			vec2 center = space_hex_to_world(
					space_hex_to_vec(hex_space_h));

			for (i32 i = 0; i < STROKE_MESH_VERTICES_PER_HEX; ++i) {
				StrokeMeshVertex *vx = &m->vertices[vi + i];
				vx->x = corners[i].x + center.x;
				vx->y = corners[i].y + center.y;
				vx->c = h.c;
				vx->r = h.r;

				Hex offset = neighbor_offsets[i];
				Hex h2 = space_hex_to_storage(
						hex_add(hex_space_h, offset));
				vx->c2 = h2.c;
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
