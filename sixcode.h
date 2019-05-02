#ifndef __SIXCODE_H__
#define __SIXCODE_H__

#include <stdbool.h>
#include <sys/types.h>

typedef unsigned int uint;

// Note: prefer int and uint unless explicit size is needed
typedef int64_t i64;
typedef u_int64_t u64;
typedef int32_t i32;
typedef u_int32_t u32;
typedef int16_t i16;
typedef u_int16_t u16;
typedef int8_t i8;
typedef u_int8_t u8;

typedef struct {
	float x;
	float y;
} vec2;

typedef struct {
	i32 x;
	i32 y;
} ivec2;

typedef struct {
	float m[2][2];
} mat2;

typedef struct {
	float m[4][4];
} mat4;

static inline ivec2 ivec2_from_vec(vec2 v) {
	return (ivec2) {v.x, v.y};
}

static inline vec2 vec2_from_ivec(ivec2 v) {
	return (vec2) {v.x, v.y};
}

// log_error is defined in test.h and electron/sixcode.c
void log_error(const char *format, ...);
#define SIXCODE_ERROR(...) log_error(__VA_ARGS__)

#endif // __SIXCODE_H__
