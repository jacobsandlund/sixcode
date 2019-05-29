#ifndef Fnv_h
#define Fnv_h

#include "spacetime.h"

u64 fnv_hash(const u8 *s, i64 length);

static inline u64 fnv_fold(u64 hash, u64 shift_bits, u64 mask)
{
	return ((hash >> shift_bits) ^ hash) & mask;
}

#endif // Fnv_h
