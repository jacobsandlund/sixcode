#include <math.h>
#include <stdlib.h>
#include "layout.h"

#define SQRT_3 1.7320508075688772

const Orientation LAYOUT_POINTY = {
	.f = {
		SQRT_3,
		SQRT_3 / 2.0,
		0.0,
		3.0 / 2.0,
	},
	.b = {
		SQRT_3 / 3.0,
		-1.0 / 3.0,
		0.0,
		2.0 / 3.0,
	},
	.start_angle = 0.5,
};

const Orientation LAYOUT_FLAT = {
	.f = {
		3.0 / 2.0,
		0.0,
		SQRT_3 / 2.0,
		SQRT_3,
	},
	.b = {
		2.0 / 3.0,
		0.0,
		-1.0 / 3.0,
		SQRT_3 / 3.0,
	},
	.start_angle = 0.0,
};

Layout *layout_create(Orientation orientation, Point viewport_size, Point translation, f64 scale)
{
	Layout *l = malloc(sizeof *l);

	l->orientation = orientation;
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
	f64 *f = l->orientation.f;
	f64 scale = l->scale;
	Point translation = l->translation;
	Point p = {
		.x = (f[0] * h.q + f[1] * h.r) * scale - translation.x,
		.y = (f[2] * h.q + f[3] * h.r) * scale - translation.y,
	};
	return p;
}

FloatHex layout_point_to_float_hex(Layout *l, Point p)
{
	f64 *b = l->orientation.b;
	f64 scale = l->scale;
	Point translation = l->translation;
	f64 x = (p.x + translation.x) / scale;
	f64 y = (p.y + translation.y) / scale;
	FloatHex h = {
		.q = b[0] * x + b[1] * y,
		.r = b[2] * x + b[3] * y,
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
	f64 start_angle = l->orientation.start_angle;

	for (i32 i = 0; i < 6; ++i) {
		f64 angle = M_PI / 3.0 * (start_angle - i);
		corners[i].x = scale * cos(angle) + center.x;
		corners[i].y = scale * sin(angle) + center.y;
	}

	return corners;
}
