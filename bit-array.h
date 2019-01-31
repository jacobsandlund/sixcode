#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include "sixcode.h"

#define BIT_ARRAY_SHIFT 6  // >> 6 is / 64
#define BIT_ARRAY_MASK 63

typedef u64 BitArray;

i32 bit_array_word_capacity(i32 bit_capacity);
i8 bit_array_has(BitArray *b, i32 i);
void bit_array_set(BitArray *b, i32 i);
void bit_array_clear(BitArray *b, i32 i);


#endif // __BIT_ARRAY_H__
