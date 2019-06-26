#ifndef _MathBasic_h
#define _MathBasic_h

#include "Spacetime.h"

static inline int2 Int2FromFloat2(float2 v)
{
    return (int2){ (i32)v.x, (i32)v.y };
}

static inline float2 Float2FromInt2(int2 v)
{
    return (float2){ (float)v.x, (float)v.y };
}

static inline int2 Int2Add(int2 a, int2 b)
{
    return (int2){ a.x + b.x, a.y + b.y };
}

static inline int2 Int2Sub(int2 a, int2 b)
{
    return (int2){ a.x - b.x, a.y - b.y };
}

#endif  // _MathBasic_h
