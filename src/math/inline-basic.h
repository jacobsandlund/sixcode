#ifndef InlineBasic_h
#define InlineBasic_h

// #included in spacetime.h

static inline int2 int2_from_float2(float2 v)
{
    return (int2) {(i32) v.x, (i32) v.y};
}

static inline float2 float2_from_int2(int2 v)
{
    return (float2) {(float) v.x, (float) v.y};
}

static inline int2 int2_add(int2 a, int2 b)
{
    return (int2) {a.x + b.x, a.y + b.y};
}

static inline int2 int2_sub(int2 a, int2 b)
{
    return (int2) {a.x - b.x, a.y - b.y};
}

#endif // InlineBasic_h
