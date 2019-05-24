#include "test.h"
#include "mesh.c"
#include "quad.c"
#include "layout.c"

Test(fill_mesh_init_hex)
{
	FillMesh *m = malloc(sizeof *m);
	fill_mesh_init(m, LayoutTypeHex, 32, 64);

	_d(m->size_x);
	//=> 32
	_d(m->size_y);
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
	//=> 0.475, 0.316667
	_gg(m->vertices[1].x, m->vertices[1].y);
	//=> 3.35849e-17, 0.633333
	_gg(m->vertices[2].x, m->vertices[2].y);
	//=> -0.475, 0.316667
	_gg(m->vertices[3].x, m->vertices[3].y);
	//=> -0.475, -0.316667
	_gg(m->vertices[4].x, m->vertices[4].y);
	//=> -1.00755e-16, -0.633333
	_gg(m->vertices[5].x, m->vertices[5].y);
	//=> 0.475, -0.316667
	_gg(m->vertices[m->vertices_length - 1].x, m->vertices[m->vertices_length - 1].y);
	//=> 31.975, 62.6833

	_dd(m->vertices[0].hx, m->vertices[0].hy);
	//=> 0, 0
	_dd(m->vertices[5].hx, m->vertices[5].hy);
	//=> 0, 0
	_dd(m->vertices[6].hx, m->vertices[6].hy);
	//=> 1, 0
	_dd(m->vertices[m->vertices_length - 1].hx, m->vertices[m->vertices_length - 1].hy);
	//=> 31, 63

	_ddd(m->indices[0], m->indices[1], m->indices[2]);
	//=> 1, 2, 0
	_ddd(m->indices[3], m->indices[4], m->indices[5]);
	//=> 0, 2, 3
	_ddd(m->indices[6], m->indices[7], m->indices[8]);
	//=> 0, 3, 5
	_ddd(m->indices[9], m->indices[10], m->indices[11]);
	//=> 3, 4, 5
	_ddd(m->indices[m->indices_length - 3], m->indices[m->indices_length - 1], m->indices[m->indices_length - 1]);
	//=> 12285, 12287, 12287

	fill_mesh_destroy(m);
	free(m);
}

Test(fill_mesh_init_rect)
{
	FillMesh *m = malloc(sizeof *m);
	fill_mesh_init(m, LayoutTypeRect, 32, 64);

	_d(m->size_x);
	//=> 32
	_d(m->size_y);
	//=> 64

	_d(32 * 64 * 4);
	//=> 8192
	_d(32 * 64 * 6);
	//=> 12288

	_d(m->vertices_length);
	//=> 8192
	_d(m->indices_length);
	//=> 12288

	_gg(m->vertices[0].x, m->vertices[0].y);
	//=> 0.475, -0.475
	_gg(m->vertices[1].x, m->vertices[1].y);
	//=> -0.475, -0.475
	_gg(m->vertices[2].x, m->vertices[2].y);
	//=> -0.475, 0.475
	_gg(m->vertices[3].x, m->vertices[3].y);
	//=> 0.475, 0.475
	_gg(m->vertices[m->vertices_length - 1].x, m->vertices[m->vertices_length - 1].y);
	//=> 31.475, 63.475

	_dd(m->vertices[0].hx, m->vertices[0].hy);
	//=> 0, 0
	_dd(m->vertices[3].hx, m->vertices[3].hy);
	//=> 0, 0
	_dd(m->vertices[4].hx, m->vertices[4].hy);
	//=> 1, 0
	_dd(m->vertices[m->vertices_length - 1].hx, m->vertices[m->vertices_length - 1].hy);
	//=> 31, 63

	_ddd(m->indices[0], m->indices[1], m->indices[2]);
	//=> 0, 1, 2
	_ddd(m->indices[3], m->indices[4], m->indices[5]);
	//=> 0, 2, 3
	_ddd(m->indices[m->indices_length - 3], m->indices[m->indices_length - 1], m->indices[m->indices_length - 1]);
	//=> 8188, 8191, 8191

	fill_mesh_destroy(m);
	free(m);
}

Test(instance_mesh)
{
	InstanceMesh *m = malloc(sizeof *m);
	instance_mesh_init(m, 1);

	_d(m->vertices_length);
	//=> 1
	_d(m->vertices_capacity);
	//=> 1
	m->vertices[0].gridPositionOffset.y = 4;

	instance_mesh_resize(m, 6);

	_d(m->vertices_length);
	//=> 6
	_d(m->vertices_capacity);
	//=> 6
	m->vertices[5].gridPositionOffset.y = 6;

	instance_mesh_resize(m, 3);

	_d(m->vertices_length);
	//=> 3
	_d(m->vertices_capacity);
	//=> 6
	_d(m->vertices[5].gridPositionOffset.y);
	//=> 6

	instance_mesh_destroy(m);
	free(m);
}
