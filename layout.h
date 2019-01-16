#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include "sixcode.h"
#include "point.h"
#include "hex.h"

typedef struct {
	Point viewport_size;
	Point translation;
	f64 scale;
} Layout;

Layout *layout_create(Point viewport_size, Point origin, f64 scale);
void layout_destroy(Layout *l);
void layout_zoom_at_point(Layout *l, Point p, f64 new_scale);
void layout_resize(Layout *l, Point viewport_size);
Point layout_hex_to_point(Layout *l, Hex h);
FloatHex layout_point_to_float_hex(Layout *l, Point p);
Hex layout_point_to_hex(Layout *l, Point p);
Point *layout_hex_corners(Point *corners, Layout *l, Hex h);

#endif // __LAYOUT_H__
