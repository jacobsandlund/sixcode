#ifndef __HEX_H__
#define __HEX_H__

#include "sixcode.h"
#include "matrix.h"

typedef struct {
	i16 c;
	i16 r;
} Hex;

Hex hex_sub(Hex a, Hex b);
Hex hex_round(vec2 h);

#endif // __HEX_H__
