#ifndef __HEX_H__
#define __HEX_H__

#include "sixcode.h"

typedef struct {
	i16 c;
	i16 r;
} Hex;

Hex hex_add(Hex a, Hex b);
Hex hex_sub(Hex a, Hex b);

#endif // __HEX_H__
