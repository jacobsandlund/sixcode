#ifndef CAMERA_H
#define CAMERA_H

#include "spacetime.h"
#include "layout.h"

void camera_translate(float3 *camera, Layout *l, float2 delta);

#endif // CAMERA_H
