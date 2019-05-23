#include <math.h>
#include <stdlib.h>
#include "viewport.h"

#define VIEWPORT_HEX_TOP_POINT_CUTOFF 0.3333333333333333
#define VIEWPORT_HEX_BOTTOM_POINT_CUTOFF 0.6666666666666666

static const double VIEWPORT_DOUBLE_EPSILON = 1e-9;

float2 viewport_screen_to_world_vector(Viewport *vp, float2 v)
{
	float2 v_moved = {
		 v.x - vp->size.x / 2.0f,
		-v.y + vp->size.y / 2.0f,
	};

	float2 v_scaled = {
		(double) v_moved.x / vp->layout.scale.x / (double) vp->camera.z,
		(double) v_moved.y / vp->layout.scale.y / (double) vp->camera.z,
	};

	return (float2) {
		v_scaled.x + vp->camera.x,
		v_scaled.y + vp->camera.y,
	};
}

void viewport_camera_translate(Viewport *vp, float2 delta)
{
	float2 delta_scaled = {
		(double) delta.x / vp->layout.scale.x / (double) vp->camera.z,
		(double) delta.y / vp->layout.scale.y / (double) vp->camera.z,
	};

	vp->camera.x += delta_scaled.x;
	vp->camera.y -= delta_scaled.y;
}

void viewport_camera_zoom_at_screen_vector(Viewport *vp, float2 v, float new_camera_scale)
{
	float2 world_vector_old = viewport_screen_to_world_vector(vp, v);
	vp->camera = (float3) {0.0f, 0.0f, new_camera_scale};
	float2 world_vector_untranslated = viewport_screen_to_world_vector(vp, v);

	vp->camera.x = world_vector_old.x - world_vector_untranslated.x;
	vp->camera.y = world_vector_old.y - world_vector_untranslated.y;
}

void viewport_to_world_quad_hex(Viewport *vp, Quad *out_q)
{
	float2 top_left_point = {-1, -1};
	float2 bottom_right_point = {
		vp->size.x + 1,
		vp->size.y + 1,
	};
	float2 top_left = viewport_screen_to_world_vector(vp, top_left_point);
	float2 bottom_right = viewport_screen_to_world_vector(vp, bottom_right_point);

	i64 top = floor(top_left.y);
	i64 double_left = floor(2.0 * top_left.x);
	i64 bottom = floor(bottom_right.y);
	i64 double_right = ceil(2.0 * (double) bottom_right.x + VIEWPORT_DOUBLE_EPSILON);

	top += top_left.y - top > VIEWPORT_HEX_BOTTOM_POINT_CUTOFF;
	bottom += bottom_right.y - bottom > VIEWPORT_HEX_TOP_POINT_CUTOFF;

	out_q->min = (int2) {(i32) (double_left >> 1), (i32) top};
	out_q->max = (int2) {(i32) (double_right >> 1), (i32) bottom};

	if (double_right - double_left <= 2 || bottom - top <= 2) {
		float2 top_right = {bottom_right.x, top_left.y};
		float2 bottom_left = {top_left.x, bottom_right.y};

		int2 top_left_hex = layout_world_vector_round_hex(top_left);
		int2 top_right_hex = layout_world_vector_round_hex(top_right);
		int2 bottom_left_hex = layout_world_vector_round_hex(bottom_left);
		int2 bottom_right_hex = layout_world_vector_round_hex(bottom_right);

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

void viewport_to_world_quad_rect(Viewport *vp, Quad *out_q)
{
	float2 top_left_point = {-1, -1};
	float2 bottom_right_point = {
		vp->size.x + 1,
		vp->size.y + 1,
	};

	out_q->min = layout_world_vector_round_rect(
			viewport_screen_to_world_vector(vp, top_left_point));
	out_q->max = layout_world_vector_round_rect(
			viewport_screen_to_world_vector(vp, bottom_right_point));
}

void viewport_to_world_quad(Viewport *vp, Quad *out_q)
{
	switch (vp->layout.type) {
	case LAYOUT_TYPE_HEX:
		viewport_to_world_quad_hex(vp, out_q);
		break;
	case LAYOUT_TYPE_RECT:
		viewport_to_world_quad_rect(vp, out_q);
		break;
	}
}
