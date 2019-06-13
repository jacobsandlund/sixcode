#include "spacetime.h"
#include "test.h"

Test(float2_int2_conversions)
{
    float2 v = {3.1, -42.7};
    _f2(v);
    //=> 3.1, -42.7

    int2 iv = int2_from_float2(v);
    _i2(iv);
    //=> 3, -42

    _f2(float2_from_int2(iv));
    //=> 3, -42
}

Test(int2_math)
{
    int2 v1 = {3, -7};
    int2 v2 = {5, 11};

    _i2(int2_add(v1, v2));
    //=> 8, 4
    _i2(int2_sub(v2, v1));
    //=> 2, 18
}
