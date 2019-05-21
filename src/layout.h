#ifndef LAYOUT_H
#define LAYOUT_H

#include "spacetime.h"

typedef enum {
	LAYOUT_HEX = 0,
	LAYOUT_RECT = 1,
} LayoutKind;

typedef struct {
	double2 scale;
	LayoutKind kind;
} Layout;

#define CAMERA_NUM_LAYOUTS 2

void layout_kind(Layout *l, LayoutKind kind);
int2 layout_world_vector_round_hex(float2 v);
int2 layout_world_vector_round_rect(float2 v);
int2 layout_world_vector_round(Layout *l, float2 v);

#endif // LAYOUT_H
