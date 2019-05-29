#include "world/vector.h"
#include <math.h>

int2 world_vector_round_hex(float2 v)
{
	double r = v.y;
	double q = (v.x * 2.0 - v.y) / 2.0;
	double s = -q - r;
	i64 qi = (i64) lround(q);
	i64 ri = (i64) lround(r);
	i64 si = (i64) lround(s);
	double q_diff = fabs(qi - q);
	double r_diff = fabs(ri - r);
	double s_diff = fabs(si - s);

	if (q_diff > r_diff && q_diff > s_diff) {
		qi = -ri - si;
	} else if (r_diff > s_diff) {
		ri = -qi - si;
	}

	return (int2) {
		(i32) (qi + (ri - (ri & 1)) / 2),
		(i32) ri,
	};
}

int2 world_vector_round_rect(float2 v)
{
	return (int2) {
		(i32) lround(v.x),
		(i32) lround(v.y),
	};
}

int2 world_vector_round(RenderLayout *rl, float2 v)
{
	switch (rl->type) {
	case RenderLayoutTypeHex:
		return world_vector_round_hex(v);
	case RenderLayoutTypeRect:
		return world_vector_round_rect(v);
	}
}
