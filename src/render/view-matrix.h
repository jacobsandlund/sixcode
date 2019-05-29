#ifndef RenderViewMatrix_h
#define RenderViewMatrix_h

#include "spacetime.h"
#include "render/viewport.h"

void render_view_matrix_init(float4x4 *vm);
void render_view_matrix_update(float4x4 *vm, RenderViewport *viewport, float3 *camera, float2 draw_offset);

#endif // RenderViewMatrix_h
