#ifndef RenderVector_h
#define RenderVector_h

#include "spacetime.h"
#include "render/viewport.h"

float2 render_vector_screen_to_world(RenderViewport *vp, float3 *camera, float2 v);
void render_vector_viewport_to_world_quad(RenderViewport *vp, float3 *camera, Quad *out_q);

#endif // RenderVector_h
