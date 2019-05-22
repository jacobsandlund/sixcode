#include "camera.c"
#include "test.h"
#include "layout.c"

TEST(camera_translate)
{
	Layout l;
	layout_type(&l, LAYOUT_TYPE_HEX);
	float3 camera = {500, -2000, 20.0};
	float2 delta = {300, 400};

	camera_translate(&camera, &l, delta);

	_f3(camera);
	//=> 508.66, -1986.67, 20
}
