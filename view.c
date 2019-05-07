#include "view.h"
#include <math.h>
#include <stdlib.h>

#define VIEW_HEX_TOP_POINT_CUTOFF 0.3333333333333333
#define VIEW_HEX_BOTTOM_POINT_CUTOFF 0.6666666666666666

static const double VIEW_DOUBLE_EPSILON = 1e-9;

void view_initialize(View *vw, vec2 viewport_size, vec2 translation, float layout_independent_scale)
{
	vw->viewport_size = viewport_size;
	vw->translation = translation;
	vw->layout_independent_scale = layout_independent_scale;
	view_layout(vw, VIEW_LAYOUT_HEX);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			vw->view_matrix.m[i][j] = 0.0f;
		}
	}
}

void view_layout(View *vw, ViewLayout layout)
{
	vw->layout = layout;

	// TODO
	switch (layout) {
	case VIEW_LAYOUT_HEX:
		vw->scale = (dvec2) {
			1.7320508075688772,	// sqrt(3)
			-1.5,			// -3.0 / 2.0
		};
		break;
	case VIEW_LAYOUT_RECT:
		vw->scale = (dvec2) {
			1.0,
			1.0,
		};
		break;
	}
}

vec2 view_screen_to_world(View *vw, vec2 v)
{
	vec2 v_moved = {
		 v.x - vw->viewport_size.x / 2.0f,
		-v.y + vw->viewport_size.y / 2.0f,
	};

	vec2 v_scaled = {
		(double) v_moved.x / vw->scale.x / (double) vw->layout_independent_scale,
		(double) v_moved.y / vw->scale.y / (double) vw->layout_independent_scale,
	};

	return (vec2) {
		v_scaled.x + vw->translation.x,
		v_scaled.y + vw->translation.y,
	};
}

void view_zoom_at_screen_point(View *vw, vec2 v, float new_layout_independent_scale)
{
	vec2 v_world_old = view_screen_to_world(vw, v);
	vw->layout_independent_scale = new_layout_independent_scale;
	vw->translation = (vec2) {0.0f, 0.0f};
	vec2 v_world_untranslated = view_screen_to_world(vw, v);

	vw->translation.x = v_world_old.x - v_world_untranslated.x;
	vw->translation.y = v_world_old.y - v_world_untranslated.y;
}

void view_resize(View *vw, vec2 viewport_size)
{
	vw->viewport_size = viewport_size;
}

void view_translate(View *vw, vec2 delta)
{
	vec2 delta_scaled = {
		(double) delta.x / vw->scale.x / (double) vw->layout_independent_scale,
		(double) delta.y / vw->scale.y / (double) vw->layout_independent_scale,
	};

	vw->translation.x += delta_scaled.x;
	vw->translation.y -= delta_scaled.y;
}

void view_update_matrix(View *vw, vec2 draw_offset)
{
	double scale_x = vw->scale.x / vw->viewport_size.x;
	double scale_y = vw->scale.y / vw->viewport_size.y;
	vw->view_matrix.m[0][0] = scale_x;
	vw->view_matrix.m[1][1] = scale_y;
	vw->view_matrix.m[3][0] = (-vw->translation.x + draw_offset.x) * scale_x;
	vw->view_matrix.m[3][1] = (-vw->translation.y + draw_offset.y) * scale_y;
	vw->view_matrix.m[3][3] = 1.0 / vw->layout_independent_scale / 2.0;
}

ivec2 view_hex_round(vec2 v)
{
	double r = v.y;
	double q = (v.x * 2.0 - v.y) / 2.0;
	double s = -q - r;
	int qi = lround(q);
	int ri = lround(r);
	int si = lround(s);
	double q_diff = fabs(qi - q);
	double r_diff = fabs(ri - r);
	double s_diff = fabs(si - s);

	if (q_diff > r_diff && q_diff > s_diff) {
		qi = -ri - si;
	} else if (r_diff > s_diff) {
		ri = -qi - si;
	}

	return (ivec2) {
		qi + (ri - (ri & 1)) / 2,
		ri,
	};
}

void view_viewport_to_quad(View *vw, Quad *out_q)
{
	vec2 top_left_point = {-1, -1};
	vec2 bottom_right_point = {
		vw->viewport_size.x + 1,
		vw->viewport_size.y + 1,
	};
	vec2 top_left = view_screen_to_world(vw, top_left_point);
	vec2 bottom_right = view_screen_to_world(vw, bottom_right_point);

	int top = floor(top_left.y);
	int double_left = floor(2.0 * top_left.x);
	int bottom = floor(bottom_right.y);
	int double_right = ceil(2.0 * (double) bottom_right.x + VIEW_DOUBLE_EPSILON);

	top += top_left.y - top > VIEW_HEX_BOTTOM_POINT_CUTOFF;
	bottom += bottom_right.y - bottom > VIEW_HEX_TOP_POINT_CUTOFF;

	out_q->min = (ivec2) {double_left >> 1, top};
	out_q->max = (ivec2) {double_right >> 1, bottom};

	if (double_right - double_left <= 2 || bottom - top <= 2) {
		vec2 top_right = {bottom_right.x, top_left.y};
		vec2 bottom_left = {top_left.x, bottom_right.y};

		ivec2 top_left_hex = view_hex_round(top_left);
		ivec2 top_right_hex = view_hex_round(top_right);
		ivec2 bottom_left_hex = view_hex_round(bottom_left);
		ivec2 bottom_right_hex = view_hex_round(bottom_right);

		if (top_left_hex.x == top_right_hex.x) {
			out_q->min.y = top_left_hex.y;
		}

		if (bottom_left_hex.x == bottom_right_hex.x) {
			out_q->max.y = bottom_left_hex.y;
		}

		if (top_left_hex.y == bottom_left_hex.y) {
			out_q->min.x = top_left_hex.x;
		}

		if (top_right_hex.y == bottom_right_hex.y) {
			out_q->max.x = top_right_hex.x;
		}
	}
}

void view_screen_points_to_quad(View *vw, Quad *out_q, vec2 v1, vec2 v2)
{
	ivec2 h1 = view_hex_round(view_screen_to_world(vw, v1));
	ivec2 h2 = view_hex_round(view_screen_to_world(vw, v2));
	quad_from_hexes(out_q, h1, h2);
}
