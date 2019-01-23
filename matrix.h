#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "sixcode.h"

typedef struct {
	f32 x;
	f32 y;
} vec2;

typedef struct {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} vec4;

typedef struct {
	f32 m[2][2];
} mat2;

typedef struct {
	f32 m[4][4];
} mat4;

vec2 mat2_multiply_v(const mat2 *m, vec2 v);

#endif // __MATRIX_H__
