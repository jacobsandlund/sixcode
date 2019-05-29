#ifndef Camera_h
#define Camera_h

#include "spacetime.h"
#include "render/viewport.h"

typedef struct {
	float3 position;
} Camera;

void camera_translate(Camera *c, RenderViewport *vp, float2 delta);
void camera_zoom_at_screen_vector(Camera *c, RenderViewport *vp, float2 v, float new_camera_scale);

#endif // Camera_h
