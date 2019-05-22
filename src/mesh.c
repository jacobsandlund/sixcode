#include "mesh.h"
#include <math.h>
#include <stdlib.h>

#define FILL_MESH_VERTICES_PER_HEX 6
#define FILL_MESH_INDICES_PER_HEX 12
#define FILL_MESH_VERTICES_PER_RECT 4
#define FILL_MESH_INDICES_PER_RECT 6
#define FILL_MESH_FRACTION 0.95

static float2 mesh_hex_corner(i64 corner)
{
	double angle = -M_PI / 3.0 * (0.5 + corner);
	Camera c;
	camera_layout(&c, camera_LAYOUT_HEX);

	return (float2) {
		cos(angle) * FILL_MESH_FRACTION / c.scale.x,
		sin(angle) * FILL_MESH_FRACTION / c.scale.y,
	};
}

void fill_mesh_initialize_hex(FillMesh *m, i64 size_x, i64 size_y)
{
	i64 num_hexes = size_x * size_y;
	m->vertices_length = FILL_MESH_VERTICES_PER_HEX * num_hexes;
	m->indices_length = FILL_MESH_INDICES_PER_HEX * num_hexes;
	m->size_x = size_x;
	m->size_y = size_y;

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

			for (i64 i = 0; i < FILL_MESH_INDICES_PER_HEX; i++) {
				m->indices[ii + i] = vi + indices_single[i];
			}

			ii += FILL_MESH_INDICES_PER_HEX;

			for (i64 i = 0; i < FILL_MESH_VERTICES_PER_HEX; i++) {
				FillMeshVertex *vx = &m->vertices[vi + i];
				vx->x = corners[i].x + center.x;
				vx->y = corners[i].y + center.y;
				vx->hx = h.x;
				vx->hy = h.y;
			}

			vi += FILL_MESH_VERTICES_PER_HEX;
		}
	}
}

void fill_mesh_initialize_rect(FillMesh *m, i64 size_x, i64 size_y)
{
	i64 num_rects = size_x * size_y;
	m->vertices_length = FILL_MESH_VERTICES_PER_RECT * num_rects;
	m->indices_length = FILL_MESH_INDICES_PER_RECT * num_rects;
	m->size_x = size_x;
	m->size_y = size_y;

	m->vertices = malloc(m->vertices_length * sizeof *m->vertices);
	m->indices = malloc(m->indices_length * sizeof *m->indices);

	float2 size = {
		0.5 * FILL_MESH_FRACTION,
		0.5 * FILL_MESH_FRACTION,
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

			for (i64 i = 0; i < FILL_MESH_INDICES_PER_RECT; i++) {
				m->indices[ii + i] = vi + indices_single[i];
			}

			ii += FILL_MESH_INDICES_PER_RECT;

			for (i64 i = 0; i < FILL_MESH_VERTICES_PER_RECT; i++) {
				FillMeshVertex *vx = &m->vertices[vi + i];
				vx->x = corners[i].x + center.x;
				vx->y = corners[i].y + center.y;
				vx->hx = h.x;
				vx->hy = h.y;
			}

			vi += FILL_MESH_VERTICES_PER_RECT;
		}
	}
}

void fill_mesh_initialize(FillMesh *m, CameraLayout layout, i64 size_x, i64 size_y)
{
	switch (layout) {
	case camera_LAYOUT_HEX:
		fill_mesh_initialize_hex(m, size_x, size_y);
		break;
	case camera_LAYOUT_RECT:
		fill_mesh_initialize_rect(m, size_x, size_y);
		break;
	}
}

void fill_mesh_terminate(FillMesh *m)
{
	free(m->vertices);
	free(m->indices);
}

void instance_mesh_initialize(InstanceMesh *m, i64 length)
{
	m->vertices = malloc(length * sizeof *m->vertices);
	m->vertices_length = length;
	m->vertices_capacity = length;
}

void instance_mesh_resize(InstanceMesh *m, i64 length)
{
	if (length > m->vertices_capacity) {
		instance_mesh_terminate(m);
		instance_mesh_initialize(m, length);
	} else {
		m->vertices_length = length;
	}
}

void instance_mesh_terminate(InstanceMesh *m)
{
	free(m->vertices);
}
