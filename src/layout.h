#ifndef Layout_h
#define Layout_h

#include "spacetime.h"

typedef enum {
	LayoutTypeHex = 0,
	LayoutTypeRect = 1,
} LayoutType;

// Using a define so that compiler can check all switch cases are present
#define LayoutTypeNumTypes 2

typedef struct {
	double2 scale;
	LayoutType type;
} Layout;

void layout_type(Layout *l, LayoutType type);
int2 layout_world_vector_round_hex(float2 v);
int2 layout_world_vector_round_rect(float2 v);
int2 layout_world_vector_round(Layout *l, float2 v);

#endif // Layout_h
