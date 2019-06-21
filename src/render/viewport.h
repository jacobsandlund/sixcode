#ifndef _RenderViewport_h
#define _RenderViewport_h

#include "spacetime.h"
#include "render/layout.h"
#include "math/quad.h"

typedef struct {
    RenderLayout layout;
    float2 size;
} RenderViewport;

#endif // _Viewport_h
