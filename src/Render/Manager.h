#ifndef _RenderManager_h
#define _RenderManager_h

#include "Gpu/View.h"
#include "Render/Layout.h"
#include "Render/Viewport.h"
#include "Spacetime.h"

typedef struct {
    RenderLayoutType layout_type;
    float2 viewport_size;
} RenderManagerConfig;

typedef struct {
    RenderViewport viewport;
    float4x4 view_matrix;
} RenderManager;

extern RenderManager gRenderManager;

void RenderManagerInit(RenderManagerConfig *config);
void RenderManagerDestroy(void);
void RenderManagerSizeChanged(GpuView *view, float2 viewport_size);

#endif  // _RenderManager_h
