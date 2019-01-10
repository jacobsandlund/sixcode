#include "test.h"
#include "quad.c"
#include "hex.c"

#define _hx(h) _dd(h.q, h.r)
#define _qd(qd) _("(%d, %d), (%d, %d)\n", qd.min.q, qd.min.r, qd.max.q, qd.max.r);

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

TEST(quad_size)
{
       Quad qd = {
               .min = {.q = -4, .r = 2},
               .max = {.q = 7, .r = 3},
       };

       _hx(quad_size(qd));
	//=> 12, 2
       //=> 12, 2
}

TEST(quad_empty)
{
	Quad qd = {
		.min = {.q = 0, .r = 0},
		.max = {.q = 1, .r = 1},
	};

	_qd(QUAD_EMPTY);
	//=> (1073741824, 1073741824), (-1073741824, -1073741824)

	_d(quad_empty(qd));
	//=> 0
	_d(quad_empty(QUAD_EMPTY));
	//=> 1

	qd.max.q = -1;
	_d(quad_empty(qd));
	//=> 1

	qd.max.q = 1;
	qd.max.r = -1;
	_d(quad_empty(qd));
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

	_qd(quad_expand(qd, h1));
	//=> (-1, 0), (2, 1)
	_qd(quad_expand(qd, h2));
	//=> (0, 0), (9, 2)

	_qd(quad_expand(QUAD_EMPTY, h1));
	//=> (-1, 0), (-1, 0)
}

TEST(quad_capacity_quad)
{
	Quad qd = {
		.min = {.q = -2, .r = 3},
		.max = {.q = 4, .r = 5},
	};
	Hex extra_capacity = {.q = 2, .r = 1};

	_hx(quad_size(qd));
	//=> 7, 3

	_qd(quad_capacity_quad(qd, extra_capacity, 0.0));
	//=> (-4, 2), (6, 6)

	_g(quad_size(qd).q * 0.3);
	//=> 2.1

	_qd(quad_capacity_quad(qd, extra_capacity, 0.3));
	//=> (-6, 1), (8, 7)
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
