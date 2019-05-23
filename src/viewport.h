#ifndef Viewport_h
#define Viewport_h

#include "spacetime.h"
#include "layout.h"
#include "quad.h"

typedef struct {
	Layout layout;
	float3 camera;
	float2 size;
} Viewport;

float2 viewport_screen_to_world_vector(Viewport *vp, float2 v);
void viewport_camera_zoom_at_screen_vector(Viewport *vp, float2 v, float new_camera_scale);
void viewport_camera_translate(Viewport *vp, float2 delta);
void viewport_to_world_quad_hex(Viewport *vp, Quad *out_q);
void viewport_to_world_quad_rect(Viewport *vp, Quad *out_q);
void viewport_to_world_quad(Viewport *vp, Quad *out_q);

#endif // Viewport_h
