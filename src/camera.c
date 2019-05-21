#include "camera.h"

void camera_translate(float3 *camera, Layout *l, float2 delta)
{
	float2 delta_scaled = {
		(double) delta.x / l->scale.x / (double) camera->z,
		(double) delta.y / l->scale.y / (double) camera->z,
	};

	camera->x += delta_scaled.x;
	camera->y -= delta_scaled.y;
}
