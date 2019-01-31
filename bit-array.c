#include <stdlib.h>
#include "bit-array.h"

i32 bit_array_word_capacity(i32 bit_capacity)
{
	return (bit_capacity >> BIT_ARRAY_SHIFT) + 1;
}

i8 bit_array_has(BitArray *b, i32 i)
{
	u64 bit = (u64) 1 << (i & BIT_ARRAY_MASK);
	return (b[i >> BIT_ARRAY_SHIFT] & bit) != (u64) 0;
}

void bit_array_set(BitArray *b, i32 i)
{
	u64 bit = (u64) 1 << (i & BIT_ARRAY_MASK);
	b[i >> BIT_ARRAY_SHIFT] |= bit;
}

void bit_array_clear(BitArray *b, i32 i)
{
	u64 bit = (u64) 1 << (i & BIT_ARRAY_MASK);
	b[i >> BIT_ARRAY_SHIFT] &= ~bit;
}
