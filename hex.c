#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "hex.h"

i32 hex_s(Hex h)
{
	return -h.q - h.r;
}

u8 hex_equal(Hex a, Hex b)
{
	return a.q == b.q && a.r == b.r;
}

Hex hex_add(Hex a, Hex b)
{
	Hex h = {.q = a.q + b.q, .r = a.r + b.r};
	return h;
}

Hex hex_subtract(Hex a, Hex b)
{
	Hex h = {.q = a.q - b.q, .r = a.r - b.r};
	return h;
}

Hex directions[] = {
	{.q = 1, .r = 0},
	{.q = 1, .r = -1},
	{.q = 0, .r = -1},
	{.q = -1, .r = 0},
	{.q = -1, .r = 1},
	{.q = 0, .r = 1},
};

Hex hex_neighbor(Hex h, u8 direction)
{
	assert(direction < 6);
	return hex_add(h, directions[direction]);
}

f64 hex_distance(Hex a, Hex b)
{
	return (f64) (abs(a.q - b.q) + abs(a.r - b.r) + abs(-a.q - a.r + b.q + b.r)) / 2.0;
}

FloatHex hex_lerp(Hex a, Hex b, f64 t)
{
	FloatHex h = {
		.q = (f64) a.q * (1.0 - t) + (f64) b.q * t,
		.r = (f64) a.r * (1.0 - t) + (f64) b.r * t,
	};
	return h;
}

Hex hex_round(FloatHex fh)
{
	Hex h;
	f64 s = -fh.q - fh.r;
	i32 qi = lround(fh.q);
	i32 ri = lround(fh.r);
	i32 si = lround(s);
	f64 q_diff = fabs(qi - fh.q);
	f64 r_diff = fabs(ri - fh.r);
	f64 s_diff = fabs(si - s);

	if (q_diff > r_diff && q_diff > s_diff) {
		qi = -ri - si;
	} else if (r_diff > s_diff) {
		ri = -qi - si;
	}

	h.q = qi;
	h.r = ri;

	return h;
}
