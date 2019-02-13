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

#ifndef SIXCODE_ERROR
	#define SIXCODE_ERROR(...) fprintf(stderr, __VA_ARGS__)
#endif

#endif // __SIXCODE_H__
