#ifndef __HEX_H__
#define __HEX_H__

#include "sixcode.h"

typedef struct {
	i16 c;
	i16 r;
} Hex;

Hex hex_round(vec2 v);
Hex hex_floor(vec2 v);
vec2 hex_to_vec(Hex h);
Hex hex_add(Hex a, Hex b);
Hex hex_sub(Hex a, Hex b);
Hex hex_to_storage(Hex h);
Hex hex_from_storage(Hex h);

#endif // __HEX_H__
