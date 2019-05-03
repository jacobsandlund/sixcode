#include "quad.c"
#include "test.h"
#include "hex-coords.c"

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

TEST(quad_contains_quad)
{
	Quad outer = {{-4, 2}, {7, 6}};
	Quad inner = {{-1, 2}, {1, 4}};

	_d(quad_contains_quad(&outer, &outer));
	//=> 1
	_d(quad_contains_quad(&outer, &inner));
	//=> 1

	inner.min = (ivec2) {-1, 1};
	_d(quad_contains_quad(&outer, &inner));
	//=> 0

	inner = (Quad) {{-4, 2}, {8, 4}};
	_d(quad_contains_quad(&outer, &inner));
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

TEST(quad_expand_for_hex)
{
	Quad out_q;
	Quad q = {{0, 0}, {127, 63}};

	ivec2 h = {-1, 75};
	quad_expand_for_hex(&out_q, &q, h);
	_qd(out_q);
	//=> (-1, 0), (127, 75)

	h = (ivec2) {253, -480};
	quad_expand_for_hex(&q, &q, h);
	_qd(q);
	//=> (0, -480), (253, 63)
}

TEST(quad_expand_for_quad)
{
	Quad out_q;
	Quad a = {{0, 0}, {127, 63}};
	Quad b = {{-1, 75}, {500, 76}};

	quad_expand_for_quad(&out_q, &a, &b);
	_qd(out_q);
	//=> (-1, 0), (500, 76)

	b = (Quad) {{-100, -3}, {-3, 48}};
	quad_expand_for_quad(&out_q, &a, &b);
	_qd(out_q);
	//=> (-100, -3), (127, 63)
}

TEST(quad_block_align)
{
	Quad out_q;
	ivec2 block_size = {128, 64};

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

TEST(quad_resize_by_delta)
{
	Quad out_q;
	Quad q = {{4, -8}, {10, 32}};
	ivec2 delta = {+2, +1};

	quad_resize_by_delta(&out_q, &q, delta);
	_qd(out_q);
	//=> (2, -9), (12, 33)

	delta = (ivec2) {-2, -1};
	quad_resize_by_delta(&out_q, &out_q, delta);
	_qd(out_q);
	//=> (4, -8), (10, 32)

	delta = (ivec2) {+4, +4};
	quad_resize_by_delta(&out_q, &q, delta);
	_qd(out_q);
	//=> (0, -12), (14, 36)
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

TEST(quad_hex_coords_to_storage)
{
	Quad storage_quad;
	Quad q1 = {{-2, 8}, {5, 18}};
	Quad q2 = {{3, 2}, {4, 3}};

	quad_hex_coords_to_storage(&storage_quad, &q1);

	_qd(storage_quad);
	//=> (-1, 8), (2, 18)

	quad_hex_coords_to_storage(&storage_quad, &q2);

	_qd(storage_quad);
	//=> (1, 2), (2, 3)
}

TEST(quad_to_size_quad)
{
	SizeQuad sq;
	Quad q = {{0, 1}, {6, 4}};

	quad_to_size_quad(&sq, &q);

	_sq(sq);
	//=> (0, 1), (7, 4)
}

TEST(quad_to_storage_size_quad)
{
	SizeQuad storage_quad;
	Quad quad = {{0, 23}, {125, 63}};

	quad_to_storage_size_quad(&storage_quad, &quad);

	_sq(storage_quad);
	//=> (0, 23), (63, 41)
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
