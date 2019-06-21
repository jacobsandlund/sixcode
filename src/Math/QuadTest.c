#include "Math/Quad.c"
#include "Test.h"

Test(QuadContains)
{
    Quad q = {
        .min = {-4, 2},
        .max = {7, 3},
    };
    int2 h = {-4, 3};

    _d(QuadContains(&q, h));
    //=> 1

    h = (int2) {-5, 3};
    _d(QuadContains(&q, h));
    //=> 0

    h = (int2) {7, 4};
    _d(QuadContains(&q, h));
    //=> 0
}

Test(QuadIntersect)
{
    Quad out_q;
    Quad a = {{-128, -256}, {255, 63}};
    Quad b = {{0, -512}, {127, 127}};
    Quad c = {{256, 128}, {511, 255}};

    QuadIntersect(&out_q, &a, &b);
    _qd(out_q);
    //=> (0, -256), (127, 63)

    QuadIntersect(&out_q, &a, &c);
    _qd(out_q);
    //=> (256, 128), (255, 63)
}

Test(QuadToSizeQuad)
{
    SizeQuad sq;
    Quad q = {{0, 1}, {6, 4}};

    QuadToSizeQuad(&sq, &q);

    _sq(sq);
    //=> (0, 1), (7, 4)
}

Test(SizeQuadEvenAlign)
{
    SizeQuad out_sq;
    SizeQuad sq = {
        .min = {-2, 3},
        .size = {10, 20},
    };

    SizeQuadEvenAlign(&out_sq, &sq);

    _sq(out_sq);
    //=> (-2, 2), (10, 21)

    sq.min = (int2) {-5, -21};
    SizeQuadEvenAlign(&out_sq, &sq);

    _sq(out_sq);
    //=> (-5, -22), (10, 21)

    sq.min = (int2) {-5, -20};
    SizeQuadEvenAlign(&out_sq, &sq);

    _sq(out_sq);
    //=> (-5, -20), (10, 20)
}

Test(SizeQuadCapacity)
{
    SizeQuad sq = {{-3, -1}, {6, 2}};

    _d(SizeQuadCapacity(&sq));
    //=> 12
}
