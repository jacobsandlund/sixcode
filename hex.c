#include "hex.h"
#include <math.h>

Hex hex_round(vec2 v)
{
	f64 r = v.y;
	f64 q = (v.x - v.y) / 2.0;
	f64 s = -q - r;
	i32 qi = lround(q);
	i32 ri = lround(r);
	i32 si = lround(s);
	f64 q_diff = fabs(qi - q);
	f64 r_diff = fabs(ri - r);
	f64 s_diff = fabs(si - s);

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
