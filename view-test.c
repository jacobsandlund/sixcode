#include "view.c"
#include "test.h"
#include "hex.c"
#include "quad.c"

TEST(view_screen_to_world)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {-100, -200},
		.scale = 10,
	};
	vec2 v1 = {125.981, 275};
	vec2 v2 = {980.3, 540};

	_v2(view_screen_to_world(&vw, v1));
	//=> -94.8038, 45

	_v2(view_screen_to_world(&vw, v2));
	//=> 76.06, -8
}

TEST(view_world_to_hex)
{
	vec2 v1 = {-94.8038, 45};
	vec2 v2 = {76.06, -8};

	_v2(view_world_to_hex(v1));
	//=> -109.47, -30

	_v2(view_world_to_hex(v2));
	//=> 87.8265, 5.33333
}

TEST(view_hex_to_world)
{
	vec2 v1 = {-109.47, 30};
	vec2 v2 = {87.8265, 5.333333333};

	_v2(view_hex_to_world(v1));
	//=> -94.8038, -45

	_v2(view_hex_to_world(v2));
	//=> 76.06, -8
}


TEST(view_zoom_at_screen_point)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {500, -2000},
		.scale = 20.0,
	};

	vec2 v = vw.viewport_size;
	view_zoom_at_screen_point(&vw, v, 30.0);
	_v2(vw.translation);
	//=> 1000, -2850
	_g(vw.scale);
	//=> 30

	v = (vec2) {300, 400};
	view_zoom_at_screen_point(&vw, v, 10.0);

	_v2(vw.translation);
	//=> 466.667, -1016.67
	_g(vw.scale);
	//=> 10
}

TEST(view_resize)
{
	View vw = {.viewport_size = {1000, 600}};
	vec2 new_size = {300, 400};

	view_resize(&vw, new_size);

	_v2(vw.viewport_size);
	//=> 300, 400
}

TEST(view_translate)
{
	View vw = {.translation = {500, -2000}};
	vec2 delta = {300, 400};

	view_translate(&vw, delta);

	_v2(vw.translation);
	//=> 800, -1600
}

TEST(view_viewport_to_quad)
{
	View vw = {
		.viewport_size = {1997, 1440},
		.translation = {1256.66, 778.438},
		.scale = 45,
	};
	Quad q;

	// Wide viewport
	view_viewport_to_quad(&vw, &q);
	_qd(q);
	//=> (13, 2), (116, 45)

	// Narrow with bottom and tops showing
	vw.translation = (vec2) {28969.1, 14133.2};
	vw.scale = 1024;

	view_viewport_to_quad(&vw, &q);
	_qd(q);
	//=> (63, 17), (68, 20)
	
	// Right and left barely showing
	vw.translation = (vec2) {40457.2, 19954.1};
	vw.scale = 1448;

	view_viewport_to_quad(&vw, &q);
	_qd(q);
	//=> (62, 18), (67, 19)

	// Almost only showing one hex
	vw.translation = (vec2) {80705.7, 39176.9};
	vw.scale = 2896;

	view_viewport_to_quad(&vw, &q);
	_qd(q);
	//=> (64, 18), (66, 19)

	// Only showing one hex
	vw.translation = (vec2) {80416.7, 39135.9};
	vw.scale = 2896;

	view_viewport_to_quad(&vw, &q);
	_qd(q);
	//=> (64, 18), (64, 18)
}

TEST(view_screen_points_to_quad)
{
	Quad q;
	View vw = {
		.viewport_size = {1997, 1440},
		.translation = {1256.66, 778.438},
		.scale = 45,
	};
	vec2 v1 = {15.3, 1040};
	vec2 v2 = {1698, 370};

	view_screen_points_to_quad(&vw, &q, v1, v2);

	_qd(q);
	//=> (14, 13), (101, 32)
}
