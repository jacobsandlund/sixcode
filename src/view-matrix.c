#include "view-matrix.h"

void view_matrix_initialize(float4x4 *vm)
{
	for (i64 i = 0; i < 4; i++) {
		for (i64 j = 0; j < 4; j++) {
			vm->columns[i][j] = 0.0f;
		}
	}
}

void view_matrix_update(float4x4 *vm, Layout *layout, float2 viewport_size, float3 camera, float2 draw_offset)
{
	double scale_x = layout->scale.x / viewport_size.x;
	double scale_y = layout->scale.y / viewport_size.y;
	vm->columns[0][0] = scale_x;
	vm->columns[1][1] = scale_y;
	vm->columns[3][0] = (draw_offset.x - camera.x) * scale_x;
	vm->columns[3][1] = (draw_offset.y - camera.y) * scale_y;
	vm->columns[3][3] = 0.5 / camera.z;
}
