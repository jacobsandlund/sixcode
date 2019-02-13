#include <math.h>
#include <stdlib.h>
#include "view.h"
#include "space.h"

#define VIEW_HEX_TOP_POINT_CUTOFF 0.3333333333333333
#define VIEW_HEX_BOTTOM_POINT_CUTOFF 0.6666666666666666

static const f64 VIEW_DOUBLE_EPSILON = 1e-9;

void view_zoom_at_screen_point(View *vw, vec2 v, f32 new_scale)
{
	vec2 v_world = space_screen_to_world(vw, v);
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
	vec2 top_left = space_world_to_hex(
			space_screen_to_world(vw, top_left_point));
	vec2 bottom_right = space_world_to_hex(
			space_screen_to_world(vw, bottom_right_point));

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

		Hex top_left_hex = space_hex_round(top_left);
		Hex top_right_hex = space_hex_round(top_right);
		Hex bottom_left_hex = space_hex_round(bottom_left);
		Hex bottom_right_hex = space_hex_round(bottom_right);

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
	Hex h1 = space_hex_round(space_world_to_hex(
			space_screen_to_world(vw, v1)));
	Hex h2 = space_hex_round(space_world_to_hex(
			space_screen_to_world(vw, v2)));
	quad_from_hexes(out_q, h1, h2);
}
