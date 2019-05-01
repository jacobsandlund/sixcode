#include "view.h"
#include <math.h>
#include <stdlib.h>

#define VIEW_HEX_TOP_POINT_CUTOFF 0.3333333333333333
#define VIEW_HEX_BOTTOM_POINT_CUTOFF 0.6666666666666666

static const f64 VIEW_DOUBLE_EPSILON = 1e-9;

vec2 view_screen_to_world(View *vw, vec2 v)
{
	vec2 v_moved = {
		 v.x - vw->viewport_size.x / 2.0f + vw->translation.x,
		-v.y + vw->viewport_size.y / 2.0f - vw->translation.y,
	};

	return (vec2) {
		(f64) v_moved.x / (f64) vw->scale * 2.0,
		(f64) v_moved.y / (f64) vw->scale * 2.0,
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

void view_zoom_at_screen_point(View *vw, vec2 v, f32 new_scale)
{
	vec2 v_world = view_screen_to_world(vw, v);
	f64 scale_factor = (f64) (new_scale - vw->scale) / 2.0;

	vw->translation.x += (f64) v_world.x * scale_factor;
	vw->translation.y -= (f64) v_world.y * scale_factor;
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

	i32 top = floor(top_left.y);
	i32 left = floor(top_left.x);
	i32 bottom = floor(bottom_right.y);
	i32 right = ceil((f64) bottom_right.x + VIEW_DOUBLE_EPSILON);

	top += top_left.y - top > VIEW_HEX_BOTTOM_POINT_CUTOFF;
	bottom += bottom_right.y - bottom > VIEW_HEX_TOP_POINT_CUTOFF;

	out_q->min = (Hex) {left, top};
	out_q->max = (Hex) {right, bottom};

	if (right - left <= 2 || bottom - top <= 2) {
		vec2 top_right = {bottom_right.x, top_left.y};
		vec2 bottom_left = {top_left.x, bottom_right.y};

		Hex top_left_hex = hex_round(top_left);
		Hex top_right_hex = hex_round(top_right);
		Hex bottom_left_hex = hex_round(bottom_left);
		Hex bottom_right_hex = hex_round(bottom_right);

		if (top_left_hex.c == top_right_hex.c) {
			out_q->min.r = top_left_hex.r;
		}

		if (bottom_left_hex.c == bottom_right_hex.c) {
			out_q->max.r = bottom_left_hex.r;
		}

		if (top_left_hex.r == bottom_left_hex.r) {
			out_q->min.c = top_left_hex.c;
		}

		if (top_right_hex.r == bottom_right_hex.r) {
			out_q->max.c = top_right_hex.c;
		}
	}
}

void view_screen_points_to_quad(View *vw, Quad *out_q, vec2 v1, vec2 v2)
{
	Hex h1 = hex_round(view_world_to_hex(
			view_screen_to_world(vw, v1)));
	Hex h2 = hex_round(view_world_to_hex(
			view_screen_to_world(vw, v2)));
	quad_from_hexes(out_q, h1, h2);
}
