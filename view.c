#include <math.h>
#include <stdlib.h>
#include "view.h"

static const mat2 VIEW_POINT_TO_HEX = {{
	{1.1547005383792517,	0.0},	// 2.0 / sqrt(3)
	{0.0,			-0.6666666666666666},	// 2.0 / 3.0
}};

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

	view_update_matrix(vw);
}

void view_update_matrix(View *vw)
{
	f64 size_x = vw->viewport_size.x;
	f64 size_y = vw->viewport_size.y;
	f64 scale_inv = 1.0 / vw->scale;
	f64 translation_x = vw->translation.x;
	f64 translation_y = vw->translation.y;

	vw->view_matrix.m[0][0] = 1.0 / size_x;
	vw->view_matrix.m[1][1] = 1.0 / size_y;
	vw->view_matrix.m[3][0] = -translation_x / size_x * scale_inv * 2.0;
	vw->view_matrix.m[3][1] =  translation_y / size_y * scale_inv * 2.0;
	vw->view_matrix.m[3][3] = scale_inv;
}

void view_zoom_at_point(View *vw, vec2 v, f32 new_scale)
{
	f64 scale_factor = (f64) (new_scale - vw->scale) / (f64) vw->scale;
	vec2 v_moved = {
		v.x - vw->viewport_size.x / 2.0f + vw->translation.x,
		v.y - vw->viewport_size.y / 2.0f + vw->translation.y,
	};

	vw->translation.x += (f64) v_moved.x * scale_factor;
	vw->translation.y += (f64) v_moved.y * scale_factor;
	vw->scale = new_scale;

	view_update_matrix(vw);
}

void view_resize(View *vw, vec2 viewport_size)
{
	vw->viewport_size = viewport_size;
	view_update_matrix(vw);
}

Hex view_point_to_hex(View *vw, vec2 v)
{
	vec2 v_moved = {
		 v.x - vw->viewport_size.x / 2.0f + vw->translation.x,
		-v.y + vw->viewport_size.y / 2.0f - vw->translation.y,
	};
	vec2 v_scaled = {
		(f64) v_moved.x / (f64) vw->scale * 2.0,
		(f64) v_moved.y / (f64) vw->scale * 2.0,
	};

	return hex_round(mat2_multiply_v(&VIEW_POINT_TO_HEX, v_scaled));
}
