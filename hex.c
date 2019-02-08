#include <math.h>
#include "hex.h"

Hex hex_sub(Hex a, Hex b)
{
	return (Hex) {a.c - b.c, a.r - b.r};
}

Hex hex_round(vec2 vec_hex)
{
	f64 r = vec_hex.y;
	f64 q = (vec_hex.x - vec_hex.y) / 2.0;
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
