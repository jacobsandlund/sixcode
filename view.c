#include "view.h"
#include <math.h>
#include <stdlib.h>
#include "hex-coords.h"

#define VIEW_HEX_TOP_POINT_CUTOFF 0.3333333333333333
#define VIEW_HEX_BOTTOM_POINT_CUTOFF 0.6666666666666666

static const double VIEW_DOUBLE_EPSILON = 1e-9;

vec2 view_screen_to_world(View *vw, vec2 v)
{
	vec2 v_moved = {
		 v.x - vw->viewport_size.x / 2.0f + vw->translation.x,
		-v.y + vw->viewport_size.y / 2.0f - vw->translation.y,
	};

	return (vec2) {
		(double) v_moved.x / (double) vw->scale * 2.0,
		(double) v_moved.y / (double) vw->scale * 2.0,
	};
}

vec2 view_world_to_hex(vec2 v)
{
	return (vec2) {
		v.x * 1.1547005383792517,	// 2.0 / sqrt(3)
		v.y * -0.6666666666666666,	// -2.0 / 3.0
	};
}

vec2 view_hex_to_world(vec2 v)
{
	return (vec2) {
		v.x * 0.8660254037844386,	// sqrt(3) / 2.0
		v.y * -1.5,			// -3.0 / 2.0
	};
}

void view_zoom_at_screen_point(View *vw, vec2 v, float new_scale)
{
	vec2 v_world = view_screen_to_world(vw, v);
	double scale_factor = (double) (new_scale - vw->scale) / 2.0;

	vw->translation.x += (double) v_world.x * scale_factor;
	vw->translation.y -= (double) v_world.y * scale_factor;
	vw->scale = new_scale;
}

void view_resize(View *vw, vec2 viewport_size)
{
	vw->viewport_size = viewport_size;
}

void view_translate(View *vw, vec2 delta)
{
	vw->translation.x += delta.x;
	vw->translation.y += delta.y;
}

void view_viewport_to_quad(View *vw, Quad *out_q)
{
	vec2 top_left_point = {-1, -1};
	vec2 bottom_right_point = {
		vw->viewport_size.x + 1,
		vw->viewport_size.y + 1,
	};
	vec2 top_left = view_world_to_hex(
			view_screen_to_world(vw, top_left_point));
	vec2 bottom_right = view_world_to_hex(
			view_screen_to_world(vw, bottom_right_point));

	int top = floor(top_left.y);
	int left = floor(top_left.x);
	int bottom = floor(bottom_right.y);
	int right = ceil((double) bottom_right.x + VIEW_DOUBLE_EPSILON);

	top += top_left.y - top > VIEW_HEX_BOTTOM_POINT_CUTOFF;
	bottom += bottom_right.y - bottom > VIEW_HEX_TOP_POINT_CUTOFF;

	out_q->min = (ivec2) {left, top};
	out_q->max = (ivec2) {right, bottom};

	if (right - left <= 2 || bottom - top <= 2) {
		vec2 top_right = {bottom_right.x, top_left.y};
		vec2 bottom_left = {top_left.x, bottom_right.y};

		ivec2 top_left_hex = hex_coords_round(top_left);
		ivec2 top_right_hex = hex_coords_round(top_right);
		ivec2 bottom_left_hex = hex_coords_round(bottom_left);
		ivec2 bottom_right_hex = hex_coords_round(bottom_right);

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
	ivec2 h1 = hex_coords_round(view_world_to_hex(
			view_screen_to_world(vw, v1)));
	ivec2 h2 = hex_coords_round(view_world_to_hex(
			view_screen_to_world(vw, v2)));
	quad_from_hexes(out_q, h1, h2);
}
