#include "hex-coords.h"
#include <math.h>

ivec2 hex_coords_round(vec2 v)
{
	double r = v.y;
	double q = (v.x - v.y) / 2.0;
	double s = -q - r;
	int qi = lround(q);
	int ri = lround(r);
	int si = lround(s);
	double q_diff = fabs(qi - q);
	double r_diff = fabs(ri - r);
	double s_diff = fabs(si - s);

	if (q_diff > r_diff && q_diff > s_diff) {
		qi = -ri - si;
	} else if (r_diff > s_diff) {
		ri = -qi - si;
	}

	return (ivec2) {(qi << 1) + ri, ri};
}

ivec2 hex_coords_to_storage(ivec2 h)
{
	return (ivec2) {h.x >> 1, h.y};
}

ivec2 hex_coords_from_storage(ivec2 h)
{
	return (ivec2) {
		(h.x << 1) + (h.y & 1),
		h.y,
	};
}
