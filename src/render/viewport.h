#ifndef RenderViewport_h
#define RenderViewport_h

#include "spacetime.h"
#include "render/layout.h"
#include "math/quad.h"

typedef struct {
	RenderLayout layout;
	float2 size;
} RenderViewport;

#endif // Viewport_h
