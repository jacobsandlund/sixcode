#include <sys/types.h>

typedef int32_t i32;
typedef uint32_t u32;
typedef int16_t i16;
typedef uint16_t u16;
typedef int8_t i8;
typedef uint8_t u8;
typedef double f64;

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>
	#define EXPORT EMSCRIPTEN_KEEPALIVE
#else
	#define EXPORT
#endif
