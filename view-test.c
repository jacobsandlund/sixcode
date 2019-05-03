#include "view.c"
#include "test.h"
#include "hex-coords.c"
#include "quad.c"

TEST(view_initialize)
{
	View *vw = malloc(sizeof *vw);
	vec2 viewport_size = {1000, 600};
	vec2 translation = {-100, -200};
	float scale = 10;
	view_initialize(vw, viewport_size, translation, scale);

	_v2(vw->viewport_size);
	//=> 1000, 600
	_v2(vw->translation);
	//=> -100, -200
	_v2(vw->scale);
	//=> 10, 10
	_g(vw->layout_independent_scale);
	//=> 10

	_gggg(vw->view_matrix.m[0][0], vw->view_matrix.m[1][0], vw->view_matrix.m[2][2], vw->view_matrix.m[3][3]);
	//=> 0, 0, 0, 0

	free(vw);
}

TEST(view_screen_to_world)
{
	View *vw = malloc(sizeof *vw);
	vec2 viewport_size = {1000, 600};
	vec2 translation = {-100, -200};
	float scale = 10;
	view_initialize(vw, viewport_size, translation, scale);

	vec2 v1 = {125.981, 275};
	vec2 v2 = {980.3, 540};

	_v2(view_screen_to_world(vw, v1));
	//=> -94.8038, 45

	_v2(view_screen_to_world(vw, v2));
	//=> 76.06, -8

	free(vw);
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
	View *vw = malloc(sizeof *vw);
	vec2 viewport_size = {1000, 600};
	vec2 translation = {500, -2000};
	float scale = 20.0;
	view_initialize(vw, viewport_size, translation, scale);

	vec2 v = vw->viewport_size;
	view_zoom_at_screen_point(vw, v, 30.0);
	_v2(vw->translation);
	//=> 1000, -2850
	_g(vw->layout_independent_scale);
	//=> 30
	_v2(vw->scale);
	//=> 30, 30

	v = (vec2) {300, 400};
	view_zoom_at_screen_point(vw, v, 10.0);

	_v2(vw->translation);
	//=> 466.667, -1016.67
	_g(vw->layout_independent_scale);
	//=> 10
	_v2(vw->scale);
	//=> 10, 10

	free(vw);
}

TEST(view_resize)
{
	View *vw = malloc(sizeof *vw);
	vec2 viewport_size = {1000, 600};
	vec2 translation = {500, -2000};
	float scale = 20.0;
	view_initialize(vw, viewport_size, translation, scale);

	vec2 new_size = {300, 400};

	view_resize(vw, new_size);

	_v2(vw->viewport_size);
	//=> 300, 400

	free(vw);
}

TEST(view_translate)
{
	View *vw = malloc(sizeof *vw);
	vec2 viewport_size = {1000, 600};
	vec2 translation = {500, -2000};
	float scale = 20.0;
	view_initialize(vw, viewport_size, translation, scale);

	vec2 delta = {300, 400};

	view_translate(vw, delta);

	_v2(vw->translation);
	//=> 800, -1600

	free(vw);
}

TEST(view_update_matrix)
{
	View *vw = malloc(sizeof *vw);
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 100};
	float scale = 10.0;
	view_initialize(vw, viewport_size, translation, scale);

	vec2 offset = {93.0f, -13.0f};
	view_update_matrix(vw, offset);

	mat4 *m = &vw->view_matrix;
	_gggg(m->m[0][0], m->m[0][1], m->m[0][2], m->m[0][3]);
	//=> 0.001, 0, 0, 0
	_gggg(m->m[1][0], m->m[1][1], m->m[1][2], m->m[1][3]);
	//=> 0, 0.00166667, 0, 0
	_gggg(m->m[2][0], m->m[2][1], m->m[2][2], m->m[2][3]);
	//=> 0, 0, 0, 0
	_gggg(m->m[3][0], m->m[3][1], m->m[3][2], m->m[3][3]);
	//=> 0.073, 0.0116667, 0, 0.1

	free(vw);
}

TEST(view_viewport_to_quad)
{
	Quad q;
	View *vw = malloc(sizeof *vw);
	vec2 viewport_size = {1997, 1440};
	vec2 translation = {1256.66, 778.438};
	float scale = 45.0;
	view_initialize(vw, viewport_size, translation, scale);

	// Wide viewport
	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (13, 2), (116, 45)

	// Narrow with bottom and tops showing
	translation = (vec2) {28969.1, 14133.2};
	scale = 1024;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (63, 17), (68, 20)
	
	// Right and left barely showing
	translation = (vec2) {40457.2, 19954.1};
	scale = 1448;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (62, 18), (67, 19)

	// Almost only showing one hex
	translation = (vec2) {80705.7, 39176.9};
	scale = 2896;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (64, 18), (66, 19)

	// Only showing one hex
	translation = (vec2) {80416.7, 39135.9};
	scale = 2896;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (64, 18), (64, 18)

	free(vw);
}

TEST(view_screen_points_to_quad)
{
	Quad q;
	View *vw = malloc(sizeof *vw);
	vec2 viewport_size = {1997, 1440};
	vec2 translation = {1256.66, 778.438};
	float scale = 45.0;
	view_initialize(vw, viewport_size, translation, scale);
	vec2 v1 = {15.3, 1040};
	vec2 v2 = {1698, 370};

	view_screen_points_to_quad(vw, &q, v1, v2);

	_qd(q);
	//=> (14, 13), (101, 32)

	free(vw);
}
