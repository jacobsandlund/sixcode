#include "test.h"
#include "bit-array.c"

TEST(bit_array_word_capacity)
{
	_d(bit_array_word_capacity(0));
	//=> 1
	_d(bit_array_word_capacity(1));
	//=> 1
	_d(bit_array_word_capacity(63));
	//=> 1
	_d(bit_array_word_capacity(64));
	//=> 2
}

TEST(bit_array_has_and_set)
{
	BitArray b[] = {0, 0, 0, 0};

	bit_array_set(b, 0);
	bit_array_set(b, 63);
	bit_array_set(b, 64);
	bit_array_set(b, 4 * 64 - 1);

	_("%llu\n", b[0]);
	//=> 9223372036854775809

	_d(bit_array_has(b, 0));
	//=> 1
	_d(bit_array_has(b, 63));
	//=> 1
	_d(bit_array_has(b, 64));
	//=> 1
	_d(bit_array_has(b, 4 * 64 - 1));
	//=> 1
	_d(bit_array_has(b, 62));
	//=> 0
	_d(bit_array_has(b, 65));
	//=> 0
	_d(bit_array_has(b, 4 * 64 - 2));
	//=> 0
}

TEST(bit_array_clear)
{
	BitArray b[] = {0, 0, 0, 0};

	// Set 4
	bit_array_set(b, 63);
	bit_array_set(b, 64);
	bit_array_set(b, 65);
	bit_array_set(b, 4 * 64 - 1);

	// Clear 2
	bit_array_clear(b, 64);
	bit_array_clear(b, 4 * 64 - 1);

	i32 count = 0;
	for (i32 i = 0; i < 4 * 64; ++i) {
		if (bit_array_has(b, i)) {
			++count;
		}
	}
	_d(count);
	//=> 2

	_d(bit_array_has(b, 63));
	//=> 1
	_d(bit_array_has(b, 64));
	//=> 0
	_d(bit_array_has(b, 65));
	//=> 1
	_d(bit_array_has(b, 4 * 64 - 1));
	//=> 0
}
