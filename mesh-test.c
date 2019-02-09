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

	_d(m->fill_vertices_length);
	//=> 12288
	_d(m->fill_indices_length);
	//=> 24576
	_d(m->stroke_vertices_length);
	//=> 12288

	// Fill

	_gg(m->fill_vertices[0].x, m->fill_vertices[0].y);
	//=> 0.866025, 0.5
	_gg(m->fill_vertices[1].x, m->fill_vertices[1].y);
	//=> 6.12323e-17, 1
	_gg(m->fill_vertices[2].x, m->fill_vertices[2].y);
	//=> -0.866025, 0.5
	_gg(m->fill_vertices[3].x, m->fill_vertices[3].y);
	//=> -0.866025, -0.5
	_gg(m->fill_vertices[4].x, m->fill_vertices[4].y);
	//=> -1.83697e-16, -1
	_gg(m->fill_vertices[5].x, m->fill_vertices[5].y);
	//=> 0.866025, -0.5
	_gg(m->fill_vertices[12287].x, m->fill_vertices[12287].y);
	//=> 55.4256, -95

	_dd(m->fill_vertices[0].c, m->fill_vertices[0].r);
	//=> 0, 0
	_dd(m->fill_vertices[5].c, m->fill_vertices[5].r);
	//=> 0, 0
	_dd(m->fill_vertices[12287].c, m->fill_vertices[12287].r);
	//=> 31, 63

	_ddd(m->fill_indices[0], m->fill_indices[1], m->fill_indices[2]);
	//=> 1, 2, 0
	_ddd(m->fill_indices[3], m->fill_indices[4], m->fill_indices[5]);
	//=> 0, 2, 3
	_ddd(m->fill_indices[6], m->fill_indices[7], m->fill_indices[8]);
	//=> 0, 3, 5
	_ddd(m->fill_indices[9], m->fill_indices[10], m->fill_indices[11]);
	//=> 5, 3, 4
	_ddd(m->fill_indices[24573], m->fill_indices[24574], m->fill_indices[24575]);
	//=> 12287, 12285, 12286

	// Stroke

	_gg(m->stroke_vertices[0].x, m->stroke_vertices[0].y);
	//=> 0.866025, 0.5
	_gg(m->stroke_vertices[1].x, m->stroke_vertices[1].y);
	//=> 6.12323e-17, 1
	_gg(m->stroke_vertices[2].x, m->stroke_vertices[2].y);
	//=> 6.12323e-17, 1
	_gg(m->stroke_vertices[3].x, m->stroke_vertices[3].y);
	//=> -0.866025, 0.5
	_gg(m->stroke_vertices[4].x, m->stroke_vertices[4].y);
	//=> -0.866025, 0.5
	_gg(m->stroke_vertices[5].x, m->stroke_vertices[5].y);
	//=> -0.866025, -0.5
	_gg(m->stroke_vertices[12287].x, m->stroke_vertices[12287].y);
	//=> 53.6936, -95

	_dd(m->stroke_vertices[0].c, m->stroke_vertices[0].r);
	//=> 0, 0
	_dd(m->stroke_vertices[0].c2, m->stroke_vertices[0].r2);
	//=> 0, -1
	_dd(m->stroke_vertices[12287].c, m->stroke_vertices[12287].r);
	//=> 31, 63
	_dd(m->stroke_vertices[12287].c2, m->stroke_vertices[12287].r2);
	//=> 30, 63

	mesh_terminate(m);
	free(m);
}
