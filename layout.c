#include <math.h>
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

Layout *layout_create(Orientation orientation, f64 scale, Point origin)
{
	Layout *l = malloc(sizeof *l);

	l->orientation = orientation;
	l->scale = scale;
	l->origin = origin;

	return l;
}

Point layout_hex_to_point(Layout *l, Hex h)
{
	f64 *f = l->orientation.f;
	f64 scale = l->scale;
	Point origin = l->origin;
	Point p = {
		.x = (f[0] * h.q + f[1] * h.r) * scale + origin.x,
		.y = (f[2] * h.q + f[3] * h.r) * scale + origin.y,
	};
	return p;
}

FloatHex layout_point_to_float_hex(Layout *l, Point p)
{
	f64 *b = l->orientation.b;
	f64 scale = l->scale;
	Point origin = l->origin;
	f64 x = (p.x - origin.x) / scale;
	f64 y = (p.y - origin.y) / scale;
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
