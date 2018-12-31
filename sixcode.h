#include <sys/types.h>

typedef int32_t i32;
typedef u_int32_t u32;
typedef int16_t i16;
typedef u_int16_t u16;
typedef int8_t i8;
typedef u_int8_t u8;
typedef double f64;

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>
	#define EXPORT EMSCRIPTEN_KEEPALIVE
#else
	#define EXPORT
#endif

#define I32_MAX 2147483647
#define I32_MIN -2147483648
