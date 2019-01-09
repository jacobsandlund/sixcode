#include "test.h"
#include "quad.c"
#include "hex.c"

#define _qd(qd) _("(%d, %d), (%d, %d)\n", qd.min.q, qd.min.r, qd.max.q, qd.max.r);

TEST(quad_zero)
{
	_qd(QUAD_ZERO);
	//=> (0, 0), (0, 0)
}

TEST(quad_contains)
{
	Quad qd = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};
	Hex h = {.q = -4, .r = 3};

	_d(quad_contains(qd, h));
	//=> 1

	h.q = -5; h.r = 3;
	_d(quad_contains(qd, h));
	//=> 0

	h.q = -4; h.r = 2;
	_d(quad_contains(qd, h));
	//=> 1

	h.q = -4; h.r = 4;
	_d(quad_contains(qd, h));
	//=> 0

	h.q = 7; h.r = 2;
	_d(quad_contains(qd, h));
	//=> 1

	h.q = 8; h.r = 3;
	_d(quad_contains(qd, h));
	//=> 0
}

TEST(quad_on_edge)
{
	Quad qd = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 4},
	};
	Hex h;

	// On edge

	h.q = -4; h.r = 2;
	_d(quad_on_edge(qd, h));
	//=> 1

	h.q = -4; h.r = 3;
	_d(quad_on_edge(qd, h));
	//=> 1

	h.q = -1; h.r = 4;
	_d(quad_on_edge(qd, h));
	//=> 1

	h.q = 7; h.r = 3;
	_d(quad_on_edge(qd, h));
	//=> 1

	// In middle

	h.q = 2; h.r = 3;
	_d(quad_on_edge(qd, h));
	//=> 0

	// Outside

	h.q = -5; h.r = 3;
	_d(quad_on_edge(qd, h));
	//=> 0

	h.q = 8; h.r = 3;
	_d(quad_on_edge(qd, h));
	//=> 0
}


TEST(quad_distance)
{
	Quad qd = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};
	Hex h = {.q = -4, .r = 3};

	_d(quad_distance(qd, h));
	//=> 0

	h.q = -5;
	_d(quad_distance(qd, h));
	//=> 1

	h.q = 11;
	_d(quad_distance(qd, h));
	//=> 4

	h.q = 0; h.r = -1;
	_d(quad_distance(qd, h));
	//=> 3

	h.r = 8;
	_d(quad_distance(qd, h));
	//=> 5

	h.q = -5; h.r = -1;
	_d(quad_distance(qd, h));
	//=> 3

	h.q = 11; h.r = -1;
	_d(quad_distance(qd, h));
	//=> 4

	h.q = 11; h.r = 8;
	_d(quad_distance(qd, h));
	//=> 5
}

TEST(quad_index)
{
	Quad qd = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};
	Hex h = {.q = -4, .r = 3};

	_d((3 - 2 + 1) * (7 - -4 + 1));
	//=> 24

	_d(quad_index(qd, qd.min));
	//=> 0
	_d(quad_index(qd, h));
	//=> 12
	_d(quad_index(qd, qd.max));
	//=> 23
}

TEST(quad_capacity)
{
	Quad qd = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};

	_d((3 - 2 + 1) * (7 - -4 + 1));
	//=> 24

	_d(quad_capacity(qd));
	//=> 24

	_d(quad_capacity(QUAD_EMPTY));
	//=> 1
}

TEST(quad_expand)
{
	Quad qd = {
		.min = {.q = 0, .r = 0},
		.max = {.q = 2, .r = 1},
	};
	Hex h1 = {.q = -1, .r = 0};
	Hex h2 = {.q = 9, .r = 2};

	_qd(qd);
	//=> (0, 0), (2, 1)

	_qd(quad_expand(qd, h1, 0.0));
	//=> (-1, 0), (2, 1)
	_qd(quad_expand(qd, h1, 0.2));
	//=> (-2, 0), (2, 1)

	_qd(quad_expand(qd, h2, 0.0));
	//=> (0, 0), (9, 2)
	_qd(quad_expand(qd, h2, 0.06));
	//=> (0, 0), (10, 2)
}

TEST(quad_move)
{
	Quad qd = {
		.min = {.q = 0, .r = 0},
		.max = {.q = 2, .r = 1},
	};
	Hex move_by = {.q = -1, .r = 7};

	_qd(quad_move(qd, move_by));
	//=> (-1, 7), (1, 8)
}

TEST(quad_equal)
{
	Quad qd = {
		.min = {.q = -2, .r = 2},
		.max = {.q = 3, .r = 4},
	};
	Quad b = {
		.min = {.q = -8, .r = 0},
		.max = {.q = 9, .r = 4},
	};

	_d(quad_equal(qd, b));
	//=> 0

	b.min = qd.min;
	_d(quad_equal(qd, b));
	//=> 0

	b.max = qd.max;
	_d(quad_equal(qd, b));
	//=> 1

	b.min.q = -8;
	_d(quad_equal(qd, b));
	//=> 0
}
