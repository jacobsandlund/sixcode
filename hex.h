#ifndef __HEX_H__
#define __HEX_H__

#include "sixcode.h"

typedef struct {
	i32 q;
	i32 r;
} Hex;

typedef struct {
	f64 q;
	f64 r;
} FloatHex;

extern const Hex hex_zero;

i32 hex_s(Hex h);
u8 hex_equal(Hex a, Hex b);
Hex hex_add(Hex a, Hex b);
Hex hex_sub(Hex a, Hex b);
Hex hex_neighbor(Hex h, u8 direction);
u32 hex_distance(Hex a, Hex b);
FloatHex hex_lerp(Hex a, Hex b, f64 t);
Hex hex_round(FloatHex fh);

#endif // __HEX_H__
