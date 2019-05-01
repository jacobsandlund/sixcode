#ifndef __SIXCODE_H__
#define __SIXCODE_H__

#include <sys/types.h>

typedef int64_t i64;
typedef u_int64_t u64;
typedef int32_t i32;
typedef u_int32_t u32;
typedef int16_t i16;
typedef u_int16_t u16;
typedef int8_t i8;
typedef u_int8_t u8;
typedef double f64;
typedef float f32;

typedef struct {
	f32 x;
	f32 y;
} vec2;

typedef struct {
	f32 m[4][4];
} mat4;

// log_error is defined in test.h and electron/sixcode.c
void log_error(const char *format, ...);
#define SIXCODE_ERROR(...) log_error(__VA_ARGS__)

#endif // __SIXCODE_H__
