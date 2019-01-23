#include <math.h>
#include <stdlib.h>
#include "view.h"

#define VIEW_C_TO_X 0.8660254037844386  // sqrt(3) / 2.0
#define VIEW_X_TO_C 1.1547005383792517  // 2.0 / sqrt(3)
#define VIEW_R_TO_Y 1.5                 // 3.0 / 2.0
#define VIEW_Y_TO_R 0.6666666666666666  // 2.0 / 3.0
#define VIEW_START_ANGLE 0.5

void view_initialize(View *vw, vec2 viewport_size, vec2 translation, f32 scale)
{
	vw->viewport_size = viewport_size;
	vw->scale = scale;
	vw->translation = translation;

	for (i32 i = 0; i < 4; i++) {
		for (i32 j = 0; j < 4; j++) {
			vw->view_matrix.m[i][j] = 0.0f;
		}
	}
}

void view_zoom_at_point(View *vw, vec2 v, f32 new_scale)
{
	vec2 translation = vw->translation;
	f64 scale_factor = (new_scale - vw->scale) / vw->scale;
	vw->translation.x += (v.x + translation.x) * scale_factor;
	vw->translation.y += (v.y + translation.y) * scale_factor;
	vw->scale = new_scale;
}

void view_resize(View *vw, vec2 viewport_size)
{
	vw->viewport_size = viewport_size;
}

void view_update_matrix(View *vw)
{
	f32 width = vw->viewport_size.x;
	f32 height = vw->viewport_size.y;
	f32 scale_inv = 1 / vw->scale;
	vec2 translation = vw->translation;

	vw->view_matrix.m[0][0] = 1 / width;
	vw->view_matrix.m[1][1] = 1 / height;
	vw->view_matrix.m[3][0] = -translation.x / width * scale_inv * 2.0f;
	vw->view_matrix.m[3][1] = translation.y / height * scale_inv * 2.0f;
	vw->view_matrix.m[3][3] = scale_inv;
}

vec2 view_hex_to_point(View *vw, Hex h)
{
	f64 scale = vw->scale;
	vec2 translation = vw->translation;
	vec2 v = {
		.x = h.c * VIEW_C_TO_X * scale - translation.x,
		.y = h.r * VIEW_R_TO_Y * scale - translation.y,
	};
	return v;
}

FloatHex view_point_to_float_hex(View *vw, vec2 v)
{
	f64 scale = vw->scale;
	vec2 translation = vw->translation;
	FloatHex h = {
		.c = (v.x + translation.x) / scale * VIEW_X_TO_C,
		.r = (v.y + translation.y) / scale * VIEW_Y_TO_R,
	};
	return h;
}
