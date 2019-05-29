#ifndef RenderLayout_h
#define RenderLayout_h

#include "spacetime.h"

typedef enum {
	RenderLayoutTypeHex = 0,
	RenderLayoutTypeRect = 1,
} RenderLayoutType;

#define RenderLayoutTypeNumTypes 2

typedef struct {
	double2 scale;
	RenderLayoutType type;
} RenderLayout;

void render_layout_type(RenderLayout *rl, RenderLayoutType type);

#endif // RenderLayout_h
