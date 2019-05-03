#include "mesh.h"
#include <math.h>
#include <stdlib.h>
#include "hex-coords.h"
#include "view.h"

#define FILL_MESH_VERTICES_PER_HEX 6
#define FILL_MESH_INDICES_PER_HEX 12
#define MESH_FILL_FRACTION 0.95

static vec2 mesh_hex_corner(int corner)
{
	double angle = M_PI / 3.0 * (0.5 + corner);
	return (vec2) {
		cos(angle) * MESH_FILL_FRACTION,
		sin(angle) * MESH_FILL_FRACTION,
	};
}

void fill_mesh_initialize(FillMesh *m, int size_x, int size_y)
{
	int num_hexes = size_x * size_y;
	m->vertices_length = FILL_MESH_VERTICES_PER_HEX * num_hexes;
	m->indices_length = FILL_MESH_INDICES_PER_HEX * num_hexes;
	m->size_x = size_x;
	m->size_y = size_y;

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

	ivec2 h;
	int vi = 0;
	int ii = 0;

	for (h.y = 0; h.y < size_y; ++h.y) {
		for (h.x = 0; h.x < size_x; ++h.x) {
			vec2 center = view_hex_to_world(
					vec2_from_ivec(
					hex_coords_from_storage(h)));

			for (int i = 0; i < FILL_MESH_INDICES_PER_HEX; ++i) {
				m->indices[ii + i] = vi + indices_single[i];
			}

			ii += FILL_MESH_INDICES_PER_HEX;

			for (int i = 0; i < FILL_MESH_VERTICES_PER_HEX; ++i) {
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

void fill_mesh_terminate(FillMesh *m)
{
	free(m->vertices);
	free(m->indices);
}

void instance_mesh_initialize(InstanceMesh *m, int length)
{
	m->vertices = malloc(length * sizeof *m->vertices);
	m->vertices_length = length;
	m->vertices_capacity = length;
}

void instance_mesh_resize(InstanceMesh *m, int length)
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
