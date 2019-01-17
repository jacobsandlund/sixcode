#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include "sixcode.h"

#define BIT_ARRAY_SHIFT 6  // >> 6 is / 64
#define BIT_ARRAY_MASK 63

typedef u64 BitArray;

BitArray *bit_array_create(i32 capacity);
void bit_array_destroy(BitArray *b);
i8 bit_array_has(BitArray *b, i32 i);
void bit_array_set(BitArray *b, i32 i);
void bit_array_clear(BitArray *b, i32 i);
i32 bit_array_count_has(BitArray *b, i32 capacity);

// Set the bit if src has bit set, but don't clear if it does not.
void bit_array_copy_set(BitArray *dest, i32 dest_i, BitArray *src, i32 src_i);


#endif // __BIT_ARRAY_H__
