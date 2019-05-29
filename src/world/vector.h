#ifndef WorldVector_h
#define WorldVector_h

#include "spacetime.h"
#include "render/layout.h"

int2 world_vector_round_hex(float2 v);
int2 world_vector_round_rect(float2 v);
int2 world_vector_round(RenderLayout *rl, float2 v);

#endif // WorldVector_h
