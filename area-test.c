#include "test.h"
#include "area.c"
#include "hex.c"

#define _ar(a) _("(%d, %d), (%d, %d)\n", a.min.q, a.min.r, a.max.q, a.max.r);

TEST(area_zero)
{
	_ar(AREA_ZERO);
	//=> (0, 0), (0, 0)
}

TEST(area_contains)
{
	Area a = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};
	Hex h = {.q = -4, .r = 3};

	_d(area_contains(a, h));
	//=> 1

	h.q = -5; h.r = 3;
	_d(area_contains(a, h));
	//=> 0

	h.q = -4; h.r = 2;
	_d(area_contains(a, h));
	//=> 1

	h.q = -4; h.r = 4;
	_d(area_contains(a, h));
	//=> 0

	h.q = 7; h.r = 2;
	_d(area_contains(a, h));
	//=> 1

	h.q = 8; h.r = 3;
	_d(area_contains(a, h));
	//=> 0
}

TEST(area_on_edge)
{
	Area a = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 4},
	};
	Hex h;

	// On edge

	h.q = -4; h.r = 2;
	_d(area_on_edge(a, h));
	//=> 1

	h.q = -4; h.r = 3;
	_d(area_on_edge(a, h));
	//=> 1

	h.q = -1; h.r = 4;
	_d(area_on_edge(a, h));
	//=> 1

	h.q = 7; h.r = 3;
	_d(area_on_edge(a, h));
	//=> 1

	// In middle

	h.q = 2; h.r = 3;
	_d(area_on_edge(a, h));
	//=> 0

	// Outside

	h.q = -5; h.r = 3;
	_d(area_on_edge(a, h));
	//=> 0

	h.q = 8; h.r = 3;
	_d(area_on_edge(a, h));
	//=> 0
}


TEST(area_distance)
{
	Area a = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};
	Hex h = {.q = -4, .r = 3};

	_d(area_distance(a, h));
	//=> 0

	h.q = -5;
	_d(area_distance(a, h));
	//=> 1

	h.q = 11;
	_d(area_distance(a, h));
	//=> 4

	h.q = 0; h.r = -1;
	_d(area_distance(a, h));
	//=> 3

	h.r = 8;
	_d(area_distance(a, h));
	//=> 5

	h.q = -5; h.r = -1;
	_d(area_distance(a, h));
	//=> 3

	h.q = 11; h.r = -1;
	_d(area_distance(a, h));
	//=> 4

	h.q = 11; h.r = 8;
	_d(area_distance(a, h));
	//=> 5
}

TEST(area_empty)
{
	_ar(AREA_EMPTY);
	//=> (1073741824, 1073741824), (-1073741824, -1073741824)
	_d(area_contains(AREA_EMPTY, HEX_ZERO));
	//=> 0
	_d(area_distance(AREA_EMPTY, HEX_ZERO));
	//=> 1073741824

	_d(area_empty(AREA_EMPTY));
	//=> 1
	_d(area_empty(AREA_ZERO));
	//=> 0
}

TEST(area_index)
{
	Area a = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};
	Hex h = {.q = -4, .r = 3};

	_d((3 - 2 + 1) * (7 - -4 + 1));
	//=> 24

	_d(area_index(a, a.min));
	//=> 0
	_d(area_index(a, h));
	//=> 12
	_d(area_index(a, a.max));
	//=> 23
}

TEST(area_capacity)
{
	Area a = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};

	_d((3 - 2 + 1) * (7 - -4 + 1));
	//=> 24

	_d(area_capacity(a));
	//=> 24

	_d(area_capacity(AREA_EMPTY));
	//=> 1
}

TEST(area_expand)
{
	Area a = {
		.min = {.q = 0, .r = 0},
		.max = {.q = 2, .r = 1},
	};
	Hex h1 = {.q = -1, .r = 0};
	Hex h2 = {.q = 9, .r = 2};

	_ar(a);
	//=> (0, 0), (2, 1)

	_ar(area_expand(a, h1, 0.0));
	//=> (-1, 0), (2, 1)
	_ar(area_expand(a, h1, 0.2));
	//=> (-2, 0), (2, 1)

	_ar(area_expand(a, h2, 0.0));
	//=> (0, 0), (9, 2)
	_ar(area_expand(a, h2, 0.06));
	//=> (0, 0), (10, 2)
}

TEST(area_move)
{
	Area a = {
		.min = {.q = 0, .r = 0},
		.max = {.q = 2, .r = 1},
	};
	Hex move_by = {.q = -1, .r = 7};

	_ar(area_move(a, move_by));
	//=> (-1, 7), (1, 8)
}

TEST(area_equal)
{
	Area a = {
		.min = {.q = -2, .r = 2},
		.max = {.q = 3, .r = 4},
	};
	Area b = {
		.min = {.q = -8, .r = 0},
		.max = {.q = 9, .r = 4},
	};

	_d(area_equal(a, b));
	//=> 0

	b.min = a.min;
	_d(area_equal(a, b));
	//=> 0

	b.max = a.max;
	_d(area_equal(a, b));
	//=> 1

	b.min.q = -8;
	_d(area_equal(a, b));
	//=> 0
}
