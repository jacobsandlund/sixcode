#include "view-matrix.c"
#include "test.h"
#include "layout.c"

TEST(view_matrix_initialize)
{
	float4x4 vm;
	view_matrix_initialize(&vm);
	_gggg(vm.columns[0][0], vm.columns[1][0], vm.columns[2][2], vm.columns[3][3]);
	//=> 0, 0, 0, 0
}

TEST(view_matrix_update)
{
	float4x4 vm;
	view_matrix_initialize(&vm);

	Layout l;
	layout_kind(&l, LAYOUT_HEX);
	float2 viewport_size = {1000, 600};
	float3 camera = {100, 100, 10.0};
	float2 offset = {93.0f, -13.0f};

	view_matrix_update(&vm, &l, viewport_size, camera, offset);

	_gggg(vm.columns[0][0], vm.columns[0][1], vm.columns[0][2], vm.columns[0][3]);
	//=> 0.00173205, 0, 0, 0
	_gggg(vm.columns[1][0], vm.columns[1][1], vm.columns[1][2], vm.columns[1][3]);
	//=> 0, -0.0025, 0, 0
	_gggg(vm.columns[2][0], vm.columns[2][1], vm.columns[2][2], vm.columns[2][3]);
	//=> 0, 0, 0, 0
	_gggg(vm.columns[3][0], vm.columns[3][1], vm.columns[3][2], vm.columns[3][3]);
	//=> -0.0121244, 0.2825, 0, 0.05
}
