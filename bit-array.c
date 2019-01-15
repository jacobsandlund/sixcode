#include <stdlib.h>
#include "bit-array.h"

BitArray *bit_array_create(i32 capacity)
{
	BitArray *b = calloc(capacity / 32 + 1, sizeof *b);
	return b;
}

void bit_array_destroy(BitArray *b)
{
	free(b);
}

i8 bit_array_has(BitArray *b, i32 i)
{
	u32 bit = 1 << (i % 32);
	return (b[i / 32] & bit) != 0;
}

void bit_array_set(BitArray *b, i32 i)
{
	u32 bit = 1 << (i % 32);
	b[i / 32] |= bit;
}

void bit_array_clear(BitArray *b, i32 i)
{
	u32 bit = 1 << (i % 32);
	b[i / 32] &= ~bit;
}

i32 bit_array_count_has(BitArray *b, i32 capacity)
{
	i32 count = 0;
	
	for (i32 i = 0; i < capacity; ++i) {
		if (bit_array_has(b, i)) {
			++count;
		}
	}

	return count;
}

// Set the bit if src has bit set, but don't clear if it does not.
void bit_array_copy_set(BitArray *dest, i32 dest_i, BitArray *src, i32 src_i)
{
	u32 dest_shift = dest_i % 32;
	dest[dest_i / 32] |= bit_array_has(src, src_i) << dest_shift;
}
