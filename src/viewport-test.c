#include "viewport.c"
#include "test.h"
#include "layout.c"
#include "quad.c"

TEST(viewport_screen_to_world_vector)
{
	Viewport vp = {
		.size = {1000, 600},
		.camera = {-100, -200, 10},
	};
	layout_type(&vp.layout, LAYOUT_TYPE_HEX);

	float2 v1 = {125.981, 275};
	float2 v2 = {980.3, 540};

	_f2(viewport_screen_to_world_vector(&vp, v1));
	//=> -121.594, -201.667

	_f2(viewport_screen_to_world_vector(&vp, v2));
	//=> -72.2699, -184
}

TEST(viewport_camera_translate)
{
	Viewport vp = {
		.size = {1000, 600},
		.camera = {500, -2000, 20.0},
	};
	layout_type(&vp.layout, LAYOUT_TYPE_HEX);
	float2 delta = {300, 400};

	viewport_camera_translate(&vp, delta);

	_f3(vp.camera);
	//=> 508.66, -1986.67, 20
}

TEST(viewport_camera_zoom_at_screen_vector)
{
	Viewport vp = {
		.size = {1000, 600},
		.camera = {500, -2000, 20.0},
	};
	layout_type(&vp.layout, LAYOUT_TYPE_HEX);

	float2 v = vp.size;
	viewport_camera_zoom_at_screen_vector(&vp, v, 30.0);
	_f3(vp.camera);
	//=> 504.811, -1996.67, 30

	v = (float2) {300, 400};
	viewport_camera_zoom_at_screen_vector(&vp, v, 10.0);

	_f3(vp.camera);
	//=> 512.509, -2001.11, 10
}

TEST(viewport_to_world_quad_hex)
{
	Quad q;
	Viewport vp = {
		.size = {2000, 1440},
		.camera = {1.64184, 0.75, 32.0},
	};
	layout_type(&vp.layout, LAYOUT_TYPE_HEX);

	// Wide viewport
	viewport_to_world_quad_hex(&vp, &q);
	_qd(q);
	//=> (-17, -14), (20, 16)

	// Narrow with bottom and tops showing
	vp.camera = (float3) {1.13244, -1.50198, 256.0};

	viewport_to_world_quad_hex(&vp, &q);
	_qd(q);
	//=> (-2, -4), (3, 1)
	
	// Right and left barely showing
	vp.camera = (float3) {1.01235, -1.98722, 1024.0};

	viewport_to_world_quad_hex(&vp, &q);
	_qd(q);
	//=> (0, -3), (2, -1)

	// Almost only showing one hex
	vp.camera = (float3) {8.54057, -3.04374, 1448.0};

	viewport_to_world_quad(&vp, &q);
	_qd(q);
	//=> (8, -4), (9, -3)

	// Only showing one hex
	vp.camera = (float3) {1.00386, -2.02635, 1448.0};

	viewport_to_world_quad(&vp, &q);
	_qd(q);
	//=> (1, -2), (1, -2)
}

TEST(viewport_to_world_quad_rect)
{
	Quad q;
	Viewport vp = {
		.size = {2000, 1440},
		.camera = {1.64184, 0.75, 32.0},
	};
	layout_type(&vp.layout, LAYOUT_TYPE_RECT);

	// Wide viewport
	viewport_to_world_quad(&vp, &q);
	_qd(q);
	//=> (-16, -14), (20, 16)

	// Narrow with bottom and tops showing
	vp.camera = (float3) {1.13244, -1.50198, 256.0};

	viewport_to_world_quad_rect(&vp, &q);
	_qd(q);
	//=> (-1, -3), (3, 0)
	
	// Right and left barely showing
	vp.camera = (float3) {1.01235, -1.98722, 1024.0};

	viewport_to_world_quad_rect(&vp, &q);
	_qd(q);
	//=> (0, -2), (2, -2)

	// Almost only showing one hex
	vp.camera = (float3) {8.54057, -3.04374, 1448.0};

	viewport_to_world_quad(&vp, &q);
	_qd(q);
	//=> (8, -3), (9, -3)

	// Only showing one hex
	vp.camera = (float3) {1.00386, -2.02635, 1448.0};

	viewport_to_world_quad(&vp, &q);
	_qd(q);
	//=> (1, -2), (1, -2)
}
