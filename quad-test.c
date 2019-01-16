#include "test.h"
#include "quad.c"
#include "hex.c"

#define _hx(h) _dd(h.c, h.r)
#define _qd(q) _("(%d, %d), (%d, %d)\n", q.min.c, q.min.r, q.max.c, q.max.r);

TEST(quad_contains)
{
	Quad q = {
		.min = {.c = -4, .r = 2},
		.max = {.c = 7, .r = 3},
	};
	Hex h = {.c = -4, .r = 2};

	_d(quad_contains(q, h));
	//=> 1

	h.c = -5; h.r = 3;
	_d(quad_contains(q, h));
	//=> 0

	h.c = -4; h.r = 4;
	_d(quad_contains(q, h));
	//=> 0

	h.c = 7; h.r = 2;
	_d(quad_contains(q, h));
	//=> 1

	h.c = 7; h.r = 1;
	_d(quad_contains(q, h));
	//=> 0

	h.c = 8; h.r = 2;
	_d(quad_contains(q, h));
	//=> 0
}

TEST(quad_on_edge)
{
	Quad q = {
		.min = {.c = -4, .r = 2},
		.max = {.c = 8, .r = 5},
	};
	Hex h;

	// On edge

	h.c = -4; h.r = 2;
	_d(quad_on_edge(q, h));
	//=> 1

	h.c = 8; h.r = 4;
	_d(quad_on_edge(q, h));
	//=> 1

	h.c = 0; h.r = 2;
	_d(quad_on_edge(q, h));
	//=> 1

	h.c = 3; h.r = 5;
	_d(quad_on_edge(q, h));
	//=> 1

	// In middle

	h.c = 1; h.r = 3;
	_d(quad_on_edge(q, h));
	//=> 0

	// Outside

	h.c = -5; h.r = 3;
	_d(quad_on_edge(q, h));
	//=> 0

	h.c = 2; h.r = 6;
	_d(quad_on_edge(q, h));
	//=> 0
}


TEST(quad_index)
{
	Quad q = {
		.min = {.c = -4, .r = 2},
		.max = {.c = 7, .r = 3},
	};
	Hex h = {.c = -4, .r = 3};

	_d((3 - 2 + 1) * ((7 - -4) / 2 + 1));
	//=> 12

	_d(quad_index(q, q.min));
	//=> 0
	_d(quad_index(q, h));
	//=> 6
	_d(quad_index(q, q.max));
	//=> 11
}

TEST(quad_capacity)
{
	Quad q = {
		.min = {.c = -4, .r = 2},
		.max = {.c = 7, .r = 3},
	};

	_d((3 - 2 + 1) * ((7 - -4) / 2 + 1));
	//=> 12

	_d(quad_capacity(q));
	//=> 12

	// Can't take the capacity of QUAD_EMPTY
	_d(quad_capacity(QUAD_EMPTY));
	//=> 1073741825
}

TEST(quad_size)
{
       Quad q = {
               .min = {.c = -4, .r = 2},
               .max = {.c = 7, .r = 3},
       };

       _hx(quad_size(q));
	//=> 12, 2
       //=> 12, 2
}

TEST(quad_empty)
{
	Quad q = {
		.min = {.c = 0, .r = 0},
		.max = {.c = 1, .r = 1},
	};

	_qd(QUAD_EMPTY);
	//=> (1073741824, 1073741824), (-1073741824, -1073741824)

	_d(quad_empty(q));
	//=> 0
	_d(quad_empty(QUAD_EMPTY));
	//=> 1

	q.max.c = -1;
	_d(quad_empty(q));
	//=> 1

	q.max.c = 1;
	q.max.r = -1;
	_d(quad_empty(q));
	//=> 1
}

TEST(quad_expand)
{
	Quad q = {
		.min = {.c = 0, .r = 0},
		.max = {.c = 2, .r = 1},
	};
	Hex h1 = {.c = -1, .r = 0};
	Hex h2 = {.c = 9, .r = 2};

	_qd(q);
	//=> (0, 0), (2, 1)

	_qd(quad_expand(q, h1));
	//=> (-1, 0), (2, 1)
	_qd(quad_expand(q, h2));
	//=> (0, 0), (9, 2)

	_qd(quad_expand(QUAD_EMPTY, h1));
	//=> (-1, 0), (-1, 0)
}

TEST(quad_capacity_quad)
{
	Quad q = {
		.min = {.c = -2, .r = 3},
		.max = {.c = 4, .r = 5},
	};
	Hex extra_capacity = {.c = 2, .r = 1};

	_hx(quad_size(q));
	//=> 7, 3

	_qd(quad_capacity_quad(q, extra_capacity, 0.0));
	//=> (-4, 2), (6, 6)

	_g(quad_size(q).c * 0.3);
	//=> 2.1

	_qd(quad_capacity_quad(q, extra_capacity, 0.3));
	//=> (-6, 1), (8, 7)
}

TEST(quad_move)
{
	Quad q = {
		.min = {.c = 0, .r = 0},
		.max = {.c = 2, .r = 1},
	};
	Hex move_by = {.c = -1, .r = 7};

	_qd(quad_move(q, move_by));
	//=> (-1, 7), (1, 8)
}
