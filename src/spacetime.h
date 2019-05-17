#ifndef SPACETIME_H
#define SPACETIME_H

// Note: prefer int and uint unless explicit size is needed
typedef unsigned int uint;


#if __APPLE__

#include <simd/simd.h>

typedef int64_t i64;
typedef uint64_t u64;
typedef int32_t i32;
typedef uint32_t u32;
typedef int16_t i16;
typedef uint16_t u16;
typedef int8_t i8;
typedef uint8_t u8;

typedef vector_float2 float2;
typedef vector_float4 float4;
typedef vector_double2 double2;
typedef vector_int2 int2;
typedef vector_uint2 uint2;
typedef matrix_float2x2 float2x2;
typedef matrix_float4x4 float4x4;

#endif

static inline int2 int2_from_float2(float2 v) {
	return (int2) {(i32) v.x, (i32) v.y};
}

static inline float2 float2_from_int2(int2 v) {
	return (float2) {(float) v.x, (float) v.y};
}

static inline int2 int2_add(int2 a, int2 b)
{
	return (int2) {a.x + b.x, a.y + b.y};
}

static inline int2 int2_sub(int2 a, int2 b)
{
	return (int2) {a.x - b.x, a.y - b.y};
}

#endif // SPACETIME_H
