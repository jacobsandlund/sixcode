#include "hex.h"

Hex hex_add(Hex a, Hex b)
{
	return (Hex) {a.c + b.c, a.r + b.r};
}

Hex hex_sub(Hex a, Hex b)
{
	return (Hex) {a.c - b.c, a.r - b.r};
}
