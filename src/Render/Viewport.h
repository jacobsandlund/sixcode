#ifndef _RenderViewport_h
#define _RenderViewport_h

#include "Math/Quad.h"
#include "Render/Layout.h"
#include "Spacetime.h"

typedef struct {
    RenderLayout layout;
    float2 size;
} RenderViewport;

static inline void RenderViewportLayout(RenderViewport *viewport,
                                        RenderLayoutType type)
{
    RenderLayoutSetType(&viewport->layout, type);
}

#endif  // _Viewport_h
