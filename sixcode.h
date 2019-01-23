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

#define I32_MAX 2147483647
#define I32_MIN -2147483648

#ifndef NULL
	#define NULL ((void*)0)
#endif

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>
	#define EXPORT_WEB EMSCRIPTEN_KEEPALIVE
#else
	#define EXPORT_WEB
#endif


#endif // __SIXCODE_H__
