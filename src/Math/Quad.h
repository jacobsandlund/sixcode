#ifndef _Quad_h
#define _Quad_h

#include "Spacetime.h"

typedef struct {
    int2 min;
    int2 max;
} Quad;

typedef struct {
    int2 min;
    int2 size;
} SizeQuad;

bool QuadContains(Quad *q, int2 h);
void QuadIntersect(Quad *out_q, Quad *a, Quad *b);
void QuadToSizeQuad(SizeQuad *out_sq, Quad *q);
void SizeQuadEvenAlign(SizeQuad *out_sq, SizeQuad *sq);
i64 SizeQuadCapacity(SizeQuad *sq);

#endif  // _Quad_h
