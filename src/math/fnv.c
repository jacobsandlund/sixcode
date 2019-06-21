#include "math/fnv.h"

#define FnvOffsetBasis 14695981039346656037U
#define FnvPrime 1099511628211

u64 FnvHash(const u8 *s, i64 length)
{
    u64 hash = (u64) FnvOffsetBasis;

    for (i32f i = 0; i < length; i++) {
        hash ^= (u64) s[i];
        hash *= (u64) FnvPrime;
    }

    return hash;
}
