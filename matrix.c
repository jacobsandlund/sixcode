#include "matrix.h"

vec2 mat2_multiply_v(const mat2 *m, vec2 v)
{
	return (vec2) {
		.x = m->m[0][0] * v.x + m->m[0][1] * v.y,
		.y = m->m[1][0] * v.x + m->m[1][1] * v.y,
	};
}
