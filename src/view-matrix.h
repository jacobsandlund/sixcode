#ifndef ViewMatrix_h
#define ViewMatrix_h

#include "spacetime.h"
#include "viewport.h"

void view_matrix_initialize(float4x4 *vm);
void view_matrix_update(float4x4 *vm, Viewport *viewport, float2 draw_offset);

#endif // ViewMatrix_h
