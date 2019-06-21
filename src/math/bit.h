#ifndef _Bit_h
#define _Bit_h

#include <assert.h>
#include "spacetime.h"

static inline u64 BitHighestBit(u64 x)
{
    return (u64) (x ? 64 - __builtin_clzl(x) : 0);
}

static inline u64 BitNextPowerOfTwo(u64 x)
{
    return ((u64) 1) << BitHighestBit(--x);
}

#endif // _Bit_h
