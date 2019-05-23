#ifndef LAYOUT_H
#define LAYOUT_H

#include "spacetime.h"

typedef enum {
	LayoutTypeHex = 0,
	LayoutTypeRect = 1,
} LayoutType;

typedef struct {
	double2 scale;
	LayoutType type;
} Layout;

void layout_type(Layout *l, LayoutType type);
int2 layout_world_vector_round_hex(float2 v);
int2 layout_world_vector_round_rect(float2 v);
int2 layout_world_vector_round(Layout *l, float2 v);

#endif // LAYOUT_H
