#include "test.h"
#include "hex.c"
#include "view.c"
#include "quad.c"
#include "matrix.c"

TEST(view_zoom_at_point)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {500, -2000},
		.scale = 20.0,
	};

	vec2 v = vw.viewport_size;
	view_zoom_at_point(&vw, v, 30.0);
	_v2(vw.translation);
	//=> 1000, -2850
	_g(vw.scale);
	//=> 30

	v = (vec2) {300, 400};
	view_zoom_at_point(&vw, v, 10.0);

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

TEST(view_point_to_hex_space)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {-100, -200},
		.scale = 10,
	};
	vec2 v1 = {125.981, 275};
	vec2 v2 = {980.3, 540};

	_v2(view_point_to_hex_space(&vw, v1));
	//=> -109.47, -30

	_v2(view_point_to_hex_space(&vw, v2));
	//=> 87.8265, 5.33333
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
