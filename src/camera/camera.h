#ifndef _Camera_h
#define _Camera_h

#include "spacetime.h"
#include "render/viewport.h"

typedef struct {
    float3 position;
} Camera;

void CameraTranslate(Camera *c, RenderViewport *vp, float2 delta);
void CameraZoomAtScreenVector(Camera *c, RenderViewport *vp, float2 v, float new_camera_scale);

#endif // _Camera_h
