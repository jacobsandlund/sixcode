#include "test.h"
#include "hex.c"
#include "quad.c"

#define _hx(h) _dd(h.c, h.r)

TEST(quad_contains)
{
	Quad q = {
		.min = {-4, 2},
		.max = {7, 3},
	};
	Hex h = {-4, 3};

	_d(quad_contains(&q, h));
	//=> 1

	h = (Hex) {-5, 3};
	_d(quad_contains(&q, h));
	//=> 0

	h = (Hex) {7, 4};
	_d(quad_contains(&q, h));
	//=> 0
}

TEST(quad_contains_quad)
{
	Quad outer = {{-4, 2}, {7, 6}};
	Quad inner = {{-1, 2}, {1, 4}};

	_d(quad_contains_quad(&outer, &outer));
	//=> 1
	_d(quad_contains_quad(&outer, &inner));
	//=> 1

	inner.min = (Hex) {-1, 1};
	_d(quad_contains_quad(&outer, &inner));
	//=> 0

	inner = (Quad) {{-4, 2}, {8, 4}};
	_d(quad_contains_quad(&outer, &inner));
	//=> 0
}

TEST(quad_is_simple)
{
	Quad q1 = {{0, 0}, {127, 63}};
	Quad q2 = {{0, 0}, {7, 4}};	// even max.r
	Quad q3 = {{-1, 0}, {5, 3}};	// odd min.c
	Quad q4 = {{0, 0}, {5, 3}};	// even max.c / 2

	_d(quad_is_simple(&q1));
	//=> 1
	_d(quad_is_simple(&q2));
	//=> 0
	_d(quad_is_simple(&q3));
	//=> 0
	_d(quad_is_simple(&q4));
	//=> 0
}

TEST(storage_quad_from_quad)
{
	StorageQuad sq;
	Quad q1 = {{-2, 8}, {5, 18}};
	Quad q2 = {{3, 2}, {4, 3}};

	storage_quad_from_quad(&sq, &q1);

	_hx(sq.min);
	//=> -1, 8
	_hx(sq.size);
	//=> 4, 11

	storage_quad_from_quad(&sq, &q2);

	_hx(sq.min);
	//=> 1, 2
	_hx(sq.size);
	//=> 2, 2
}

TEST(storage_quad_capacity)
{
	StorageQuad sq = {{-3, -1}, {6, 2}};

	_d(storage_quad_capacity(&sq));
	//=> 12
}
