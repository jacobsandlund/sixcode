#ifndef _RenderVector_h
#define _RenderVector_h

#include "Spacetime.h"
#include "Render/Viewport.h"

float2 RenderVectorScreenToWorld(RenderViewport *vp, float3 *camera, float2 v);
void RenderVectorViewportToWorldQuad(RenderViewport *vp, float3 *camera, Quad *out_q);

#endif // _RenderVector_h
