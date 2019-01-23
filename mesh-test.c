#include "test.h"
#include "matrix.c"
#include "mesh.c"

TEST(mesh)
{
	Mesh *m = malloc(sizeof *m);
	mesh_initialize(m, 32, 64);

	_d(32 * 64 * 6);
	//=> 12288
	_d(32 * 64 * 12);
	//=> 24576

	_d(m->vertices_length);
	//=> 12288
	_d(m->fill_indices_length);
	//=> 24576

	_gg(m->vertices[0].x, m->vertices[0].y);
	//=> 0.866025, -0.5
	_gg(m->vertices[1].x, m->vertices[1].y);
	//=> 6.12323e-17, -1
	_gg(m->vertices[2].x, m->vertices[2].y);
	//=> 0.866025, 0.5
	_gg(m->vertices[3].x, m->vertices[3].y);
	//=> -0.866025, -0.5
	_gg(m->vertices[4].x, m->vertices[4].y);
	//=> -1.83697e-16, 1
	_gg(m->vertices[5].x, m->vertices[5].y);
	//=> -0.866025, 0.5

	_gg(m->vertices[12287].x, m->vertices[12287].y);
	//=> 53.6936, 95

	_ddd(m->fill_indices[0], m->fill_indices[1], m->fill_indices[2]);
	//=> 0, 1, 2
	_ddd(m->fill_indices[3], m->fill_indices[4], m->fill_indices[5]);
	//=> 2, 1, 3
	_ddd(m->fill_indices[6], m->fill_indices[7], m->fill_indices[8]);
	//=> 2, 3, 4
	_ddd(m->fill_indices[9], m->fill_indices[10], m->fill_indices[11]);
	//=> 4, 3, 5

	_ddd(m->fill_indices[24573], m->fill_indices[24574], m->fill_indices[24575]);
	//=> 12286, 12285, 12287

	mesh_terminate(m);
	free(m);
}
