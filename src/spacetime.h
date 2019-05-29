#ifndef Spacetime_h
#define Spacetime_h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef int_fast32_t i32f;
typedef uint_fast32_t u32f;

typedef int64_t i64;
typedef uint64_t u64;
typedef int32_t i32;
typedef uint32_t u32;
typedef int16_t i16;
typedef uint16_t u16;
typedef int8_t i8;
typedef uint8_t u8;

typedef u64 sid;	// String id

#if __APPLE__

#include <simd/simd.h>

typedef vector_float2 float2;
typedef vector_float3 float3;
typedef vector_float4 float4;
typedef vector_double2 double2;
typedef vector_int2 int2;
typedef vector_uint2 uint2;
typedef matrix_float2x2 float2x2;
typedef matrix_float4x4 float4x4;

#else

typedef struct {
	float x;
	float y;
} float2;

typedef struct {
	float x;
	float y;
	float z;
} float3;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} float4;

typedef struct {
	double x;
	double y;
} double2;

typedef struct {
	i32 x;
	i32 y;
} int2;

typedef struct {
	u32 x;
	u32 y;
} uint2;

typedef struct {
	float columns[2][2];
} float2x2;

typedef struct {
	float columns[4][4];
} float4x4;

#endif

#include "math/inline-basic.h"

#ifdef Test

#define Log(...) test_log(__VA_ARGS__)
#define ILog(...) test_log(__VA_ARGS__)
#define DLog(...) /* */

#elif defined(__APPLE__)

#include <os/log.h>

extern os_log_t os_spacetime_default_log;

#define Log(...) os_log(os_spacetime_default_log, __VA_ARGS__)
#define ILog(...) os_log_info(os_spacetime_default_log, __VA_ARGS__)

#ifdef DEBUG
#define DLog(...) os_log_debug(os_spacetime_default_log, __VA_ARGS__)
#else
#define DLog(...) /* */
#endif

#endif  // __APPLE__

#endif // Spacetime_h
