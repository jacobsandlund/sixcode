#ifndef __HEX_H__
#define __HEX_H__

#include "sixcode.h"

typedef struct {
	i32 c;
	i32 r;
} Hex;

typedef struct {
	f64 c;
	f64 r;
} FloatHex;

extern const Hex HEX_ZERO;

i8 hex_equal(Hex a, Hex b);
Hex hex_add(Hex a, Hex b);
Hex hex_sub(Hex a, Hex b);
Hex hex_neighbor(Hex h, i8 direction);
i32 hex_distance(Hex a, Hex b);
FloatHex hex_lerp(Hex a, Hex b, f64 t);
Hex hex_round(FloatHex fh);
FloatHex hex_to_float_hex(Hex h);

#endif // __HEX_H__
