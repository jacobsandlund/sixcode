#include "test.h"
#include "math/bit.h"

Test(bit_highest_bit)
{
	_u64(bit_highest_bit(0));
	//=> 0
	_u64(bit_highest_bit(1));
	//=> 1
	_u64(bit_highest_bit(2));
	//=> 2
	_u64(bit_highest_bit(3));
	//=> 2
	_u64(bit_highest_bit(63));
	//=> 6
	_u64(bit_highest_bit(64));
	//=> 7
	_u64(bit_highest_bit(4294967295));
	//=> 32
	_u64(bit_highest_bit(4294967296));
	//=> 33
}

Test(bit_next_power_of_two)
{
	_u64(bit_next_power_of_two(0));
	//=> 1
	_u64(bit_next_power_of_two(1));
	//=> 1
	_u64(bit_next_power_of_two(2));
	//=> 2
	_u64(bit_next_power_of_two(3));
	//=> 4
	_u64(bit_next_power_of_two(17));
	//=> 32
	_u64(bit_next_power_of_two(64));
	//=> 64
	_u64(bit_next_power_of_two(4294967295));
	//=> 4294967296
	_u64(bit_next_power_of_two(4294967296));
	//=> 4294967296
	_u64(bit_next_power_of_two(4294967297));
	//=> 8589934592
}
