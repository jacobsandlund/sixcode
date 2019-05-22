#include <math.h>
#include "layout.h"

void layout_type(Layout *l, LayoutType type)
{
	l->type = type;
	l->scale = (double2) {
		1.7320508075688772,	// sqrt(3)
		-1.5,			// -3.0 / 2.0
	};
}

int2 layout_world_vector_round_hex(float2 v)
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

int2 layout_world_vector_round_rect(float2 v)
{
	return (int2) {
		(i32) lround(v.x),
		(i32) lround(v.y),
	};
}

int2 layout_world_vector_round(Layout *l, float2 v)
{
	switch (l->type) {
	case LAYOUT_TYPE_HEX:
		return layout_world_vector_round_hex(v);
	case LAYOUT_TYPE_RECT:
		return layout_world_vector_round_rect(v);
	}
}
