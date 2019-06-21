#include "Render/ViewMatrix.h"

void RenderViewMatrixInit(float4x4 *vm)
{
    for (i64 i = 0; i < 4; i++) {
        for (i64 j = 0; j < 4; j++) {
            vm->columns[i][j] = 0.0f;
        }
    }
}

void RenderViewMatrixUpdate(float4x4 *vm, RenderViewport *viewport, float3 *camera, float2 draw_offset)
{
    double scale_x = viewport->layout.scale.x / viewport->size.x;
    double scale_y = viewport->layout.scale.y / viewport->size.y;
    vm->columns[0][0] = scale_x;
    vm->columns[1][1] = scale_y;
    vm->columns[3][0] = (draw_offset.x - camera->x) * scale_x;
    vm->columns[3][1] = (draw_offset.y - camera->y) * scale_y;
    vm->columns[3][3] = 0.5 / camera->z;
}
