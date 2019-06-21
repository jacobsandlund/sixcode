#ifndef _RenderViewMatrix_h
#define _RenderViewMatrix_h

#include "spacetime.h"
#include "render/viewport.h"

void RenderViewMatrixInit(float4x4 *vm);
void RenderViewMatrixUpdate(float4x4 *vm, RenderViewport *viewport, float3 *camera, float2 draw_offset);

#endif // _RenderViewMatrix_h
