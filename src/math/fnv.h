#ifndef _Fnv_h
#define _Fnv_h

#include "spacetime.h"

u64 FnvHash(const u8 *s, i64 length);

static inline u64 FnvFold(u64 hash, u64 shift_bits, u64 mask)
{
    return ((hash >> shift_bits) ^ hash) & mask;
}

#endif // _Fnv_h
