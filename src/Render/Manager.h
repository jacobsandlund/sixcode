#ifndef _RenderManager_h
#define _RenderManager_h

#include "Gpu/View.h"
#include "Render/Layout.h"
#include "Render/Renderer.h"
#include "Render/Viewport.h"
#include "Spacetime.h"

typedef struct {
    RendererConfig renderer;
    RenderLayoutType layout_type;
} RenderManagerConfig;

typedef struct {
    RenderViewport viewport;
    Renderer renderer;
    float4x4 view_matrix;
} RenderManager;

extern RenderManager gRenderManager;

void RenderManagerInit(RenderManagerConfig *config);
void RenderManagerDestroy(void);
void RenderManagerDrawInView(GpuView *view);
void RenderManagerSizeChanged(GpuView *view, float2 viewport_size);

#endif  // _RenderManager_h
