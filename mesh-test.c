#include "test.h"
#include "matrix.c"
#include "mesh.c"

TEST(fill_mesh)
{
	FillMesh *m = malloc(sizeof *m);
	fill_mesh_initialize(m, 32, 64);

	_d(m->size_c);
	//=> 32
	_d(m->size_r);
	//=> 64

	_d(32 * 64 * 6);
	//=> 12288
	_d(32 * 64 * 12);
	//=> 24576

	_d(m->vertices_length);
	//=> 12288
	_d(m->indices_length);
	//=> 24576

	_gg(m->vertices[0].x, m->vertices[0].y);
	//=> 0.866025, 0.5
	_gg(m->vertices[1].x, m->vertices[1].y);
	//=> 6.12323e-17, 1
	_gg(m->vertices[2].x, m->vertices[2].y);
	//=> -0.866025, 0.5
	_gg(m->vertices[3].x, m->vertices[3].y);
	//=> -0.866025, -0.5
	_gg(m->vertices[4].x, m->vertices[4].y);
	//=> -1.83697e-16, -1
	_gg(m->vertices[5].x, m->vertices[5].y);
	//=> 0.866025, -0.5
	_gg(m->vertices[12287].x, m->vertices[12287].y);
	//=> 55.4256, -95

	_dd(m->vertices[0].c, m->vertices[0].r);
	//=> 0, 0
	_dd(m->vertices[5].c, m->vertices[5].r);
	//=> 0, 0
	_dd(m->vertices[12287].c, m->vertices[12287].r);
	//=> 31, 63

	_ddd(m->indices[0], m->indices[1], m->indices[2]);
	//=> 1, 2, 0
	_ddd(m->indices[3], m->indices[4], m->indices[5]);
	//=> 0, 2, 3
	_ddd(m->indices[6], m->indices[7], m->indices[8]);
	//=> 0, 3, 5
	_ddd(m->indices[9], m->indices[10], m->indices[11]);
	//=> 5, 3, 4
	_ddd(m->indices[24573], m->indices[24574], m->indices[24575]);
	//=> 12287, 12285, 12286

	fill_mesh_terminate(m);
	free(m);
}

TEST(stroke_mesh)
{
	StrokeMesh *m = malloc(sizeof *m);
	stroke_mesh_initialize(m, 32, 64);

	_d(m->size_c);
	//=> 32
	_d(m->size_r);
	//=> 64

	_d(32 * 64 * 6);
	//=> 12288

	_d(m->vertices_length);
	//=> 12288

	_gg(m->vertices[0].x, m->vertices[0].y);
	//=> 0.866025, 0.5
	_gg(m->vertices[1].x, m->vertices[1].y);
	//=> 6.12323e-17, 1
	_gg(m->vertices[2].x, m->vertices[2].y);
	//=> 6.12323e-17, 1
	_gg(m->vertices[3].x, m->vertices[3].y);
	//=> -0.866025, 0.5
	_gg(m->vertices[4].x, m->vertices[4].y);
	//=> -0.866025, 0.5
	_gg(m->vertices[5].x, m->vertices[5].y);
	//=> -0.866025, -0.5
	_gg(m->vertices[12287].x, m->vertices[12287].y);
	//=> 53.6936, -95

	_dd(m->vertices[0].c, m->vertices[0].r);
	//=> 0, 0
	_dd(m->vertices[0].c2, m->vertices[0].r2);
	//=> 0, -1
	_dd(m->vertices[12287].c, m->vertices[12287].r);
	//=> 31, 63
	_dd(m->vertices[12287].c2, m->vertices[12287].r2);
	//=> 30, 63

	stroke_mesh_terminate(m);
	free(m);
}
