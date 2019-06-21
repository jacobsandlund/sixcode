#include "math/bit.h"
#include "test.h"

Test(BitHighestBit)
{
    _u64(BitHighestBit(0));
    //=> 0
    _u64(BitHighestBit(1));
    //=> 1
    _u64(BitHighestBit(2));
    //=> 2
    _u64(BitHighestBit(3));
    //=> 2
    _u64(BitHighestBit(63));
    //=> 6
    _u64(BitHighestBit(64));
    //=> 7
    _u64(BitHighestBit(4294967295));
    //=> 32
    _u64(BitHighestBit(4294967296));
    //=> 33
}

Test(BitNextPowerOfTwo)
{
    _u64(BitNextPowerOfTwo(0));
    //=> 1
    _u64(BitNextPowerOfTwo(1));
    //=> 1
    _u64(BitNextPowerOfTwo(2));
    //=> 2
    _u64(BitNextPowerOfTwo(3));
    //=> 4
    _u64(BitNextPowerOfTwo(17));
    //=> 32
    _u64(BitNextPowerOfTwo(64));
    //=> 64
    _u64(BitNextPowerOfTwo(4294967295));
    //=> 4294967296
    _u64(BitNextPowerOfTwo(4294967296));
    //=> 4294967296
    _u64(BitNextPowerOfTwo(4294967297));
    //=> 8589934592
}
