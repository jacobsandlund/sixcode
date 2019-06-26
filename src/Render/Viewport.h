#ifndef _RenderViewport_h
#define _RenderViewport_h

#include "Math/Quad.h"
#include "Render/Layout.h"
#include "Spacetime.h"

typedef struct {
    RenderLayout layout;
    float2 size;
} RenderViewport;

#endif  // _Viewport_h
