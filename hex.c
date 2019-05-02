#include "hex.h"
#include <math.h>

Hex hex_round(vec2 v)
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

	return (Hex) {(qi << 1) + ri, ri};
}

Hex hex_floor(vec2 v)
{
	return (Hex) {
		floor(v.x),
		floor(v.y),
	};
}

vec2 hex_to_vec(Hex h)
{
	return (vec2) {h.c, h.r};
}

Hex hex_to_storage(Hex h)
{
	return (Hex) {h.c >> 1, h.r};
}

Hex hex_from_storage(Hex h)
{
	return (Hex) {
		(h.c << 1) + (h.r & 1),
		h.r,
	};
}

Hex hex_add(Hex a, Hex b)
{
	return (Hex) {a.c + b.c, a.r + b.r};
}

Hex hex_sub(Hex a, Hex b)
{
	return (Hex) {a.c - b.c, a.r - b.r};
}
