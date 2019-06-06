#ifndef Bit_h
#define Bit_h

#include <assert.h>
#include "spacetime.h"

static inline u64 bit_highest_bit(u64 x)
{
    return (u64) (x ? 64 - __builtin_clzl(x) : 0);
}

static inline u64 bit_next_power_of_two(u64 x)
{
    return ((u64) 1) << bit_highest_bit(--x);
}

#endif // Bit_h
