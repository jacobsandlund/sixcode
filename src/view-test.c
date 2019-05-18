#include "view.c"
#include "test.h"
#include "quad.c"

TEST(view_initialize)
{
	View *vw = malloc(sizeof *vw);
	float2 viewport_size = {1000, 600};
	float2 translation = {-100, -200};
	float scale = 10;
	view_initialize(vw, viewport_size, translation, scale);

	_v2(vw->viewport_size);
	//=> 1000, 600
	_v2(vw->translation);
	//=> -100, -200
	_v2(vw->scale);
	//=> 1.73205, -1.5
	_g(vw->layout_independent_scale);
	//=> 10
	_d(vw->layout == VIEW_LAYOUT_HEX);
	//=> 1

	_gggg(vw->view_matrix.columns[0][0], vw->view_matrix.columns[1][0], vw->view_matrix.columns[2][2], vw->view_matrix.columns[3][3]);
	//=> 0, 0, 0, 0

	free(vw);
}

TEST(view_screen_to_world)
{
	View *vw = malloc(sizeof *vw);
	float2 viewport_size = {1000, 600};
	float2 translation = {-100, -200};
	float scale = 10;
	view_initialize(vw, viewport_size, translation, scale);

	float2 v1 = {125.981, 275};
	float2 v2 = {980.3, 540};

	_v2(view_screen_to_world(vw, v1));
	//=> -121.594, -201.667

	_v2(view_screen_to_world(vw, v2));
	//=> -72.2699, -184

	free(vw);
}

TEST(view_zoom_at_screen_point)
{
	View *vw = malloc(sizeof *vw);
	float2 viewport_size = {1000, 600};
	float2 translation = {500, -2000};
	float scale = 20.0;
	view_initialize(vw, viewport_size, translation, scale);

	float2 v = vw->viewport_size;
	view_zoom_at_screen_point(vw, v, 30.0);
	_v2(vw->translation);
	//=> 504.811, -1996.67
	_g(vw->layout_independent_scale);
	//=> 30
	_v2(vw->scale);
	//=> 1.73205, -1.5

	v = (float2) {300, 400};
	view_zoom_at_screen_point(vw, v, 10.0);

	_v2(vw->translation);
	//=> 512.509, -2001.11
	_g(vw->layout_independent_scale);
	//=> 10
	_v2(vw->scale);
	//=> 1.73205, -1.5

	free(vw);
}

TEST(view_resize)
{
	View *vw = malloc(sizeof *vw);
	float2 viewport_size = {1000, 600};
	float2 translation = {500, -2000};
	float scale = 20.0;
	view_initialize(vw, viewport_size, translation, scale);

	float2 new_size = {300, 400};

	view_resize(vw, new_size);

	_v2(vw->viewport_size);
	//=> 300, 400

	free(vw);
}

TEST(view_translate)
{
	View *vw = malloc(sizeof *vw);
	float2 viewport_size = {1000, 600};
	float2 translation = {500, -2000};
	float scale = 20.0;
	view_initialize(vw, viewport_size, translation, scale);

	float2 delta = {300, 400};

	view_translate(vw, delta);

	_v2(vw->translation);
	//=> 508.66, -1986.67

	free(vw);
}

TEST(view_update_matrix)
{
	View *vw = malloc(sizeof *vw);
	float2 viewport_size = {1000, 600};
	float2 translation = {100, 100};
	float scale = 10.0;
	view_initialize(vw, viewport_size, translation, scale);

	float2 offset = {93.0f, -13.0f};
	view_update_matrix(vw, offset);

	float4x4 *m = &vw->view_matrix;
	_gggg(m->columns[0][0], m->columns[0][1], m->columns[0][2], m->columns[0][3]);
	//=> 0.00173205, 0, 0, 0
	_gggg(m->columns[1][0], m->columns[1][1], m->columns[1][2], m->columns[1][3]);
	//=> 0, -0.0025, 0, 0
	_gggg(m->columns[2][0], m->columns[2][1], m->columns[2][2], m->columns[2][3]);
	//=> 0, 0, 0, 0
	_gggg(m->columns[3][0], m->columns[3][1], m->columns[3][2], m->columns[3][3]);
	//=> -0.0121244, 0.2825, 0, 0.05

	free(vw);
}

TEST(view_world_round_hex)
{
	View *vw = malloc(sizeof *vw);
	view_layout(vw, VIEW_LAYOUT_HEX);

	float2 a = {0, 0};
	float2 b = {0.5, -1};
	float2 c = {-0.5, -1};
	float2 d = {-14.5, 63};
	float2 closer_to_a = {
		a.x * 0.4 + b.x * 0.3 + c.x * 0.3,
		a.y * 0.4 + b.y * 0.3 + c.y * 0.3,
	};
	float2 closer_to_b = {
		a.x * 0.3 + b.x * 0.4 + c.x * 0.3,
		a.y * 0.3 + b.y * 0.4 + c.y * 0.3,
	};
	float2 closer_to_c = {
		a.x * 0.3 + b.x * 0.3 + c.x * 0.4,
		a.y * 0.3 + b.y * 0.3 + c.y * 0.4,
	};
	float2 between_c_and_d_1 = {
		a.x * 0.501 + d.x * 0.499,
		a.y * 0.501 + d.y * 0.499,
	};
	float2 between_c_and_d_2 = {
		a.x * 0.449 + d.x * 0.501,
		a.y * 0.449 + d.y * 0.501,
	};

	_i2(view_world_round(vw, closer_to_a));
	//=> 0, 0
	_i2(view_world_round(vw, closer_to_b));
	//=> 0, -1
	_i2(view_world_round(vw, closer_to_c));
	//=> -1, -1
	_i2(view_world_round(vw, between_c_and_d_1));
	//=> -8, 31
	_i2(view_world_round(vw, between_c_and_d_2));
	//=> -7, 32

	free(vw);
}

TEST(view_world_round_rect)
{
	View *vw = malloc(sizeof *vw);
	view_layout(vw, VIEW_LAYOUT_RECT);

	float2 a = {0, 0};
	float2 b = {1, -1};
	float2 c = {0, -1};
	float2 d = {-15, 63};
	float2 closer_to_a = {
		a.x * 0.6 + b.x * 0.4,
		a.y * 0.6 + b.y * 0.4,
	};
	float2 closer_to_b = {
		a.x * 0.4 + b.x * 0.6,
		a.y * 0.4 + b.y * 0.6,
	};
	float2 closer_to_c = {
		b.x * 0.4 + c.x * 0.6,
		b.y * 0.4 + c.y * 0.6,
	};
	float2 between_c_and_d_1 = {
		a.x * 0.501 + d.x * 0.499,
		a.y * 0.501 + d.y * 0.499,
	};
	float2 between_c_and_d_2 = {
		a.x * 0.449 + d.x * 0.501,
		a.y * 0.449 + d.y * 0.501,
	};

	_i2(view_world_round(vw, closer_to_a));
	//=> 0, 0
	_i2(view_world_round(vw, closer_to_b));
	//=> 1, -1
	_i2(view_world_round(vw, closer_to_c));
	//=> 0, -1
	_i2(view_world_round(vw, between_c_and_d_1));
	//=> -7, 31
	_i2(view_world_round(vw, between_c_and_d_2));
	//=> -8, 32

	free(vw);
}

TEST(view_viewport_to_quad_hex)
{
	Quad q;
	View *vw = malloc(sizeof *vw);
	float2 viewport_size = {2000, 1440};
	float2 translation = {1.64184, 0.75};
	float scale = 32.0;
	view_initialize(vw, viewport_size, translation, scale);

	// Wide viewport
	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (-17, -14), (20, 16)

	// Narrow with bottom and tops showing
	translation = (float2) {1.13244, -1.50198};
	scale = 256;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (-2, -4), (3, 1)
	
	// Right and left barely showing
	translation = (float2) {1.01235, -1.98722};
	scale = 1024;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (0, -3), (2, -1)

	// Almost only showing one hex
	translation = (float2) {8.54057, -3.04374};
	scale = 1448;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (8, -4), (9, -3)

	// Only showing one hex
	translation = (float2) {1.00386, -2.02635};
	scale = 1448;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (1, -2), (1, -2)

	free(vw);
}

TEST(view_viewport_to_quad_rect)
{
	Quad q;
	View *vw = malloc(sizeof *vw);
	float2 viewport_size = {2000, 1440};
	float2 translation = {1.64184, 0.75};
	float scale = 32.0;
	view_initialize(vw, viewport_size, translation, scale);
	view_layout(vw, VIEW_LAYOUT_RECT);

	// Wide viewport
	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (-16, -14), (20, 16)

	// Narrow with bottom and tops showing
	translation = (float2) {1.13244, -1.50198};
	scale = 256;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (-2, -4), (3, 1)
	
	// Right and left barely showing
	translation = (float2) {1.01235, -1.98722};
	scale = 1024;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (0, -3), (2, -1)

	// Almost only showing one hex
	translation = (float2) {8.54057, -3.04374};
	scale = 1448;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (8, -4), (9, -3)

	// Only showing one hex
	translation = (float2) {1.00386, -2.02635};
	scale = 1448;
	view_initialize(vw, viewport_size, translation, scale);

	view_viewport_to_quad(vw, &q);
	_qd(q);
	//=> (1, -2), (1, -2)

	free(vw);
}
