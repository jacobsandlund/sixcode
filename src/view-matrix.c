#include "view-matrix.h"

void view_matrix_initialize(float4x4 *vm)
{
	for (i64 i = 0; i < 4; i++) {
		for (i64 j = 0; j < 4; j++) {
			vm->columns[i][j] = 0.0f;
		}
	}
}

void view_matrix_update(float4x4 *vm, Viewport *viewport, float2 draw_offset)
{
	double scale_x = viewport->layout.scale.x / viewport->size.x;
	double scale_y = viewport->layout.scale.y / viewport->size.y;
	vm->columns[0][0] = scale_x;
	vm->columns[1][1] = scale_y;
	vm->columns[3][0] = (draw_offset.x - viewport->camera.x) * scale_x;
	vm->columns[3][1] = (draw_offset.y - viewport->camera.y) * scale_y;
	vm->columns[3][3] = 0.5 / viewport->camera.z;
}
