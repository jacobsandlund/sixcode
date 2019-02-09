#include "test.h"
#include "hex.c"
#include "quad.c"

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

TEST(quad_expand_quad)
{
	Quad out_q;
	Quad q = {{0, 0}, {127, 63}};

	Hex h = {-1, 75};
	quad_expand_quad(&out_q, &q, h);
	_qd(out_q);
	//=> (-1, 0), (127, 75)

	h = (Hex) {253, -480};
	quad_expand_quad(&q, &q, h);
	_qd(q);
	//=> (0, -480), (253, 63)
}

TEST(quad_block_align)
{
	Quad out_q;
	Hex block_size = {128, 64};

	Quad q = {{1, 0}, {48, 13}};
	quad_block_align(&out_q, &q, block_size);
	_qd(out_q);
	//=> (0, 0), (127, 63)

	q = (Quad) {{-150, -310}, {-60, -129}};
	quad_block_align(&q, &q, block_size);
	_qd(q);
	//=> (-256, -320), (-1, -129)

	q = (Quad) {{-240, -1}, {500, 1020}};
	quad_block_align(&out_q, &q, block_size);
	_qd(out_q);
	//=> (-256, -64), (511, 1023)
}

TEST(quad_resize)
{
	Quad out_q;
	Quad q = {{4, -8}, {10, 32}};

	quad_resize(&out_q, &q, +1);
	_qd(out_q);
	//=> (2, -9), (12, 33)

	quad_resize(&out_q, &out_q, -1);
	_qd(out_q);
	//=> (4, -8), (10, 32)

	quad_resize(&out_q, &q, +4);
	_qd(out_q);
	//=> (-4, -12), (18, 36)
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

TEST(storage_quad_even_align)
{
	StorageQuad out_sq;
	StorageQuad sq = {
		.min = {-2, 3},
		.size = {10, 20},
	};

	storage_quad_even_align(&out_sq, &sq);

	_hx(out_sq.min);
	//=> -2, 2
	_hx(out_sq.size);
	//=> 10, 21

	sq.min = (Hex) {-5, -21};
	storage_quad_even_align(&out_sq, &sq);

	_hx(out_sq.min);
	//=> -5, -22
	_hx(out_sq.size);
	//=> 10, 21

	sq.min = (Hex) {-5, -20};
	storage_quad_even_align(&out_sq, &sq);

	_hx(out_sq.min);
	//=> -5, -20
	_hx(out_sq.size);
	//=> 10, 20
}

TEST(storage_quad_capacity)
{
	StorageQuad sq = {{-3, -1}, {6, 2}};

	_d(storage_quad_capacity(&sq));
	//=> 12
}
