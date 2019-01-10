#include "test.h"
#include "bit-array.c"

TEST(bit_array_create_and_destroy)
{
	BitArray *b = bit_array_create(123);

	_d(b[0]);
	//=> 0
	_d(b[123 / 64]);
	//=> 0

	bit_array_destroy(b);
}

TEST(bit_array_has_and_set)
{
	BitArray b[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	_d(bit_array_has(b, 0));
	//=> 0

	bit_array_set(b, 0);
	bit_array_set(b, 63);

	_("%ld\n", b[0]);
	//=> -9223372036854775807

	_d(bit_array_has(b, 0));
	//=> 1
	_d(bit_array_has(b, 63));
	//=> 1

	bit_array_set(b, 64);
	bit_array_set(b, 235);
	bit_array_set(b, 639);

	_d(bit_array_has(b, 63));
	//=> 1
	_d(bit_array_has(b, 64));
	//=> 1
	_d(bit_array_has(b, 235));
	//=> 1
	_d(bit_array_has(b, 639));
	//=> 1

	_d(bit_array_has(b, 62));
	//=> 0
	_d(bit_array_has(b, 65));
	//=> 0
	_d(bit_array_has(b, 234));
	//=> 0
	_d(bit_array_has(b, 236));
	//=> 0
	_d(bit_array_has(b, 638));
	//=> 0
}

TEST(bit_array_clear)
{
	BitArray b[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Set 7
	bit_array_set(b, 62);
	bit_array_set(b, 63);
	bit_array_set(b, 64);
	bit_array_set(b, 65);
	bit_array_set(b, 235);
	bit_array_set(b, 638);
	bit_array_set(b, 639);

	// Clear 4
	bit_array_clear(b, 63);
	bit_array_clear(b, 64);
	bit_array_clear(b, 235);
	bit_array_clear(b, 638);

	_d(bit_array_count_has(b, 640));
	//=> 3

	_d(bit_array_has(b, 62));
	//=> 1
	_d(bit_array_has(b, 63));
	//=> 0
	_d(bit_array_has(b, 64));
	//=> 0
	_d(bit_array_has(b, 65));
	//=> 1
	_d(bit_array_has(b, 235));
	//=> 0
	_d(bit_array_has(b, 638));
	//=> 0
	_d(bit_array_has(b, 639));
	//=> 1
}

TEST(bit_array_count_has)
{
	BitArray b[] = {0, 0, 0, 0};

	bit_array_set(b, 0);
	bit_array_set(b, 3);
	bit_array_set(b, 63);
	bit_array_set(b, 138);  // don't count this

	_d(bit_array_count_has(b, 64));
	//=> 3
}

TEST(bit_array_copy_set)
{
	BitArray a[] = {0, 0};
	BitArray b[] = {0, 0, 0, 0};

	bit_array_set(a, 0);
	bit_array_set(a, 5);
	bit_array_set(a, 65);

	bit_array_set(b, 128);

	bit_array_copy_set(b, 129, a, 0);
	bit_array_copy_set(b, 130, a, 1);
	bit_array_copy_set(b, 221, a, 64);
	bit_array_copy_set(b, 222, a, 65);

	_d(bit_array_has(b, 128));
	//=> 1
	_d(bit_array_has(b, 129));
	//=> 1
	_d(bit_array_has(b, 130));
	//=> 0
	_d(bit_array_has(b, 221));
	//=> 0
	_d(bit_array_has(b, 222));
	//=> 1

	bit_array_set(b, 33);
	_d(bit_array_has(a, 42));
	//=> 0
	bit_array_copy_set(b, 33, a, 42);

	// Don't clear even if src does not have bit set
	_d(bit_array_has(b, 33));
	//=> 1
}
