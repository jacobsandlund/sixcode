#ifndef VIEW_MATRIX_H
#define VIEW_MATRIX_H

#include "spacetime.h"
#include "viewport.h"

void view_matrix_initialize(float4x4 *vm);
void view_matrix_update(float4x4 *vm, Viewport *viewport, float2 draw_offset);

#endif // VIEW_MATRIX_H
