#include "quad.c"
#include "test.h"

TEST(quad_contains)
{
	Quad q = {
		.min = {-4, 2},
		.max = {7, 3},
	};
	ivec2 h = {-4, 3};

	_d(quad_contains(&q, h));
	//=> 1

	h = (ivec2) {-5, 3};
	_d(quad_contains(&q, h));
	//=> 0

	h = (ivec2) {7, 4};
	_d(quad_contains(&q, h));
	//=> 0
}

TEST(quad_from_hexes)
{
	Quad q;
	ivec2 h1 = {700, -3};
	ivec2 h2 = {-6, 8};

	quad_from_hexes(&q, h1, h2);
	_qd(q);
	//=> (-6, -3), (700, 8)

	quad_from_hexes(&q, h2, h1);
	_qd(q);
	//=> (-6, -3), (700, 8)
}

TEST(quad_intersect)
{
	Quad out_q;
	Quad a = {{-128, -256}, {255, 63}};
	Quad b = {{0, -512}, {127, 127}};
	Quad c = {{256, 128}, {511, 255}};

	quad_intersect(&out_q, &a, &b);
	_qd(out_q);
	//=> (0, -256), (127, 63)

	quad_intersect(&out_q, &a, &c);
	_qd(out_q);
	//=> (256, 128), (255, 63)
}

TEST(quad_to_size_quad)
{
	SizeQuad sq;
	Quad q = {{0, 1}, {6, 4}};

	quad_to_size_quad(&sq, &q);

	_sq(sq);
	//=> (0, 1), (7, 4)
}

TEST(size_quad_even_align)
{
	SizeQuad out_sq;
	SizeQuad sq = {
		.min = {-2, 3},
		.size = {10, 20},
	};

	size_quad_even_align(&out_sq, &sq);

	_sq(out_sq);
	//=> (-2, 2), (10, 21)

	sq.min = (ivec2) {-5, -21};
	size_quad_even_align(&out_sq, &sq);

	_sq(out_sq);
	//=> (-5, -22), (10, 21)

	sq.min = (ivec2) {-5, -20};
	size_quad_even_align(&out_sq, &sq);

	_sq(out_sq);
	//=> (-5, -20), (10, 20)
}

TEST(size_quad_capacity)
{
	SizeQuad sq = {{-3, -1}, {6, 2}};

	_d(size_quad_capacity(&sq));
	//=> 12
}
