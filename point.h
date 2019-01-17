#ifndef __POINT_H__
#define __POINT_H__

#include "sixcode.h"

typedef struct {
	f64 x;
	f64 y;
} Point;

typedef struct {
	Point min;
	Point max;
} PointQuad;

//PointQuad points_to_point_quad(Point a, Point b);

#endif // __POINT_H__
