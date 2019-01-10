#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include "sixcode.h"
#include "point.h"
#include "hex.h"

typedef struct {
	f64 f[4];
	f64 b[4];
	f64 start_angle;
} Orientation;

typedef struct {
	Orientation orientation;
	f64 scale;
	Point origin;
} Layout;

extern const Orientation LAYOUT_POINTY;
extern const Orientation LAYOUT_FLAT;

Layout *layout_create(Orientation orientation, f64 scale, Point origin);
void layout_destroy(Layout *l);
Point layout_hex_to_point(Layout *l, Hex h);
FloatHex layout_point_to_float_hex(Layout *l, Point p);
Hex layout_point_to_hex(Layout *l, Point p);
Point *layout_hex_corners(Point *corners, Layout *l, Hex h);

#endif // __LAYOUT_H__
