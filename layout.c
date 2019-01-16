#include <math.h>
#include <stdlib.h>
#include "layout.h"

#define LAYOUT_C_TO_X 0.8660254037844386  // sqrt(3) / 2.0
#define LAYOUT_X_TO_C 1.1547005383792517  // 2.0 / sqrt(3)
#define LAYOUT_R_TO_Y 1.5                 // 3.0 / 2.0
#define LAYOUT_Y_TO_R 0.6666666666666666  // 2.0 / 3.0
#define LAYOUT_START_ANGLE 0.5

Layout *layout_create(Point viewport_size, Point translation, f64 scale)
{
	Layout *l = malloc(sizeof *l);

	l->viewport_size = viewport_size;
	l->scale = scale;
	l->translation = translation;

	return l;
}

void layout_destroy(Layout *l)
{
	free(l);
}

void layout_zoom_at_point(Layout *l, Point p, f64 new_scale)
{
	Point translation = l->translation;
	f64 scale_factor = (new_scale - l->scale) / l->scale;
	l->translation.x += (p.x + translation.x) * scale_factor;
	l->translation.y += (p.y + translation.y) * scale_factor;
	l->scale = new_scale;
}

Point layout_hex_to_point(Layout *l, Hex h)
{
	f64 scale = l->scale;
	Point translation = l->translation;
	Point p = {
		.x = h.c * LAYOUT_C_TO_X * scale - translation.x,
		.y = h.r * LAYOUT_R_TO_Y * scale - translation.y,
	};
	return p;
}

FloatHex layout_point_to_float_hex(Layout *l, Point p)
{
	f64 scale = l->scale;
	Point translation = l->translation;
	FloatHex h = {
		.c = (p.x + translation.x) / scale * LAYOUT_X_TO_C,
		.r = (p.y + translation.y) / scale * LAYOUT_Y_TO_R,
	};
	return h;
}

Hex layout_point_to_hex(Layout *l, Point p)
{
	return hex_round(layout_point_to_float_hex(l, p));
}

Point *layout_hex_corners(Point *corners, Layout *l, Hex h)
{
	f64 scale = l->scale;
	Point center = layout_hex_to_point(l, h);

	for (i32 i = 0; i < 6; ++i) {
		f64 angle = M_PI / 3.0 * (LAYOUT_START_ANGLE - i);
		corners[i].x = scale * cos(angle) + center.x;
		corners[i].y = scale * sin(angle) + center.y;
	}

	return corners;
}
