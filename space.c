#include <math.h>
#include "space.h"

vec2 space_screen_to_world(View *vw, vec2 v)
{
	vec2 v_moved = {
		 v.x - vw->viewport_size.x / 2.0f + vw->translation.x,
		-v.y + vw->viewport_size.y / 2.0f - vw->translation.y,
	};

	return (vec2) {
		(f64) v_moved.x / (f64) vw->scale * 2.0,
		(f64) v_moved.y / (f64) vw->scale * 2.0,
	};
}

vec2 space_world_to_hex(vec2 v)
{
	return (vec2) {
		v.x * 1.1547005383792517,	// 2.0 / sqrt(3)
		v.y * -0.6666666666666666,	// -2.0 / 3.0
	};
}

vec2 space_hex_to_world(vec2 v)
{
	return (vec2) {
		v.x * 0.8660254037844386,	// sqrt(3) / 2.0
		v.y * -1.5,			// -3.0 / 2.0
	};
}

Hex space_hex_round(vec2 v)
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

vec2 space_hex_to_vec(Hex h)
{
	return (vec2) {h.c, h.r};
}

Hex space_hex_to_storage(Hex h)
{
	return (Hex) {h.c >> 1, h.r};
}

Hex space_storage_to_hex(Hex h)
{
	return (Hex) {
		(h.c << 1) + (h.r & 1),
		h.r,
	};
}
