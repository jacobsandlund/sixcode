#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "hex.h"

const Hex HEX_ZERO = {.c = 0, .r = 0};

i8 hex_equal(Hex a, Hex b)
{
	return a.c == b.c && a.r == b.r;
}

Hex hex_add(Hex a, Hex b)
{
	Hex h = {.c = a.c + b.c, .r = a.r + b.r};
	return h;
}

Hex hex_sub(Hex a, Hex b)
{
	Hex h = {.c = a.c - b.c, .r = a.r - b.r};
	return h;
}

Hex directions[] = {
	{.c = 2, .r = 0},
	{.c = 1, .r = -1},
	{.c = -1, .r = -1},
	{.c = -2, .r = 0},
	{.c = -1, .r = 1},
	{.c = 1, .r = 1},
};

Hex hex_neighbor(Hex h, i8 direction)
{
	assert(direction < 6 && direction >= 0);
	return hex_add(h, directions[direction]);
}

i32 hex_distance(Hex a, Hex b)
{
	i32 dx = abs(a.c - b.c);
	i32 dy = abs(a.r - b.r);
	i32 dx_sub_dy = dx - dy;
	return dx_sub_dy > 0 ? dy + (dx_sub_dy >> 1) : dy;
}

FloatHex hex_lerp(Hex a, Hex b, f64 t)
{
	FloatHex h = {
		.c = (f64) a.c * (1.0 - t) + (f64) b.c * t,
		.r = (f64) a.r * (1.0 - t) + (f64) b.r * t,
	};
	return h;
}

Hex hex_round(FloatHex fh)
{
	Hex h;
	f64 r = fh.r;
	f64 q = (fh.c - fh.r) / 2.0;
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

	h.c = qi * 2 + ri;
	h.r = ri;

	return h;
}

FloatHex hex_to_float_hex(Hex h)
{
	FloatHex fh = {.c = h.c, .r = h.r};
	return fh;
}
