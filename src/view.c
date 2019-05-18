#include "view.h"
#include <math.h>
#include <stdlib.h>

#define VIEW_HEX_TOP_POINT_CUTOFF 0.3333333333333333
#define VIEW_HEX_BOTTOM_POINT_CUTOFF 0.6666666666666666

static const double VIEW_DOUBLE_EPSILON = 1e-9;

void view_initialize(View *vw, float2 viewport_size, float2 translation, float layout_independent_scale)
{
	vw->viewport_size = viewport_size;
	vw->translation = translation;
	vw->layout_independent_scale = layout_independent_scale;
	view_layout(vw, VIEW_LAYOUT_HEX);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			vw->view_matrix.columns[i][j] = 0.0f;
		}
	}
}

void view_layout(View *vw, ViewLayout layout)
{
	vw->layout = layout;
	vw->scale = (double2) {
		1.7320508075688772,	// sqrt(3)
		-1.5,			// -3.0 / 2.0
	};
}

float2 view_screen_to_world(View *vw, float2 v)
{
	float2 v_moved = {
		 v.x - vw->viewport_size.x / 2.0f,
		-v.y + vw->viewport_size.y / 2.0f,
	};

	float2 v_scaled = {
		(double) v_moved.x / vw->scale.x / (double) vw->layout_independent_scale,
		(double) v_moved.y / vw->scale.y / (double) vw->layout_independent_scale,
	};

	return (float2) {
		v_scaled.x + vw->translation.x,
		v_scaled.y + vw->translation.y,
	};
}

void view_zoom_at_screen_point(View *vw, float2 v, float new_layout_independent_scale)
{
	float2 v_world_old = view_screen_to_world(vw, v);
	vw->layout_independent_scale = new_layout_independent_scale;
	vw->translation = (float2) {0.0f, 0.0f};
	float2 v_world_untranslated = view_screen_to_world(vw, v);

	vw->translation.x = v_world_old.x - v_world_untranslated.x;
	vw->translation.y = v_world_old.y - v_world_untranslated.y;
}

void view_resize(View *vw, float2 viewport_size)
{
	vw->viewport_size = viewport_size;
}

void view_translate(View *vw, float2 delta)
{
	float2 delta_scaled = {
		(double) delta.x / vw->scale.x / (double) vw->layout_independent_scale,
		(double) delta.y / vw->scale.y / (double) vw->layout_independent_scale,
	};

	vw->translation.x += delta_scaled.x;
	vw->translation.y -= delta_scaled.y;
}

void view_update_matrix(View *vw, float2 draw_offset)
{
	double scale_x = vw->scale.x / vw->viewport_size.x;
	double scale_y = vw->scale.y / vw->viewport_size.y;
	vw->view_matrix.columns[0][0] = scale_x;
	vw->view_matrix.columns[1][1] = scale_y;
	vw->view_matrix.columns[3][0] = (-vw->translation.x + draw_offset.x) * scale_x;
	vw->view_matrix.columns[3][1] = (-vw->translation.y + draw_offset.y) * scale_y;
	vw->view_matrix.columns[3][3] = 1.0 / vw->layout_independent_scale / 2.0;
}

int2 view_world_round_hex(float2 v)
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

	return (int2) {
		qi + (ri - (ri & 1)) / 2,
		ri,
	};
}

int2 view_world_round_rect(float2 v)
{
	return (int2) {
		lround(v.x),
		lround(v.y),
	};
}

int2 view_world_round(View *vw, float2 v)
{
	switch (vw->layout) {
	case VIEW_LAYOUT_HEX:
		return view_world_round_hex(v);
	case VIEW_LAYOUT_RECT:
		return view_world_round_rect(v);
	}
}

void view_viewport_to_quad_hex(View *vw, Quad *out_q)
{
	float2 top_left_point = {-1, -1};
	float2 bottom_right_point = {
		vw->viewport_size.x + 1,
		vw->viewport_size.y + 1,
	};
	float2 top_left = view_screen_to_world(vw, top_left_point);
	float2 bottom_right = view_screen_to_world(vw, bottom_right_point);

	int top = floor(top_left.y);
	int double_left = floor(2.0 * top_left.x);
	int bottom = floor(bottom_right.y);
	int double_right = ceil(2.0 * (double) bottom_right.x + VIEW_DOUBLE_EPSILON);

	top += top_left.y - top > VIEW_HEX_BOTTOM_POINT_CUTOFF;
	bottom += bottom_right.y - bottom > VIEW_HEX_TOP_POINT_CUTOFF;

	out_q->min = (int2) {double_left >> 1, top};
	out_q->max = (int2) {double_right >> 1, bottom};

	if (double_right - double_left <= 2 || bottom - top <= 2) {
		float2 top_right = {bottom_right.x, top_left.y};
		float2 bottom_left = {top_left.x, bottom_right.y};

		int2 top_left_hex = view_world_round_hex(top_left);
		int2 top_right_hex = view_world_round_hex(top_right);
		int2 bottom_left_hex = view_world_round_hex(bottom_left);
		int2 bottom_right_hex = view_world_round_hex(bottom_right);

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

void view_viewport_to_quad_rect(View *vw, Quad *out_q)
{
	float2 top_left_point = {-1, -1};
	float2 bottom_right_point = {
		vw->viewport_size.x + 1,
		vw->viewport_size.y + 1,
	};

	out_q->min = view_world_round_rect(
			view_screen_to_world(vw, top_left_point));
	out_q->max = view_world_round_rect(
			view_screen_to_world(vw, bottom_right_point));
}

void view_viewport_to_quad(View *vw, Quad *out_q)
{
	switch (vw->layout) {
	case VIEW_LAYOUT_HEX:
		view_viewport_to_quad_hex(vw, out_q);
		break;
	case VIEW_LAYOUT_RECT:
		view_viewport_to_quad_rect(vw, out_q);
		break;
	}
}
