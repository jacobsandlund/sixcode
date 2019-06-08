#ifndef RenderManager_h
#define RenderManager_h

#include "spacetime.h"
#include "render/layout.h"
#include "render/viewport.h"
#include "gpu/view.h"

typedef struct {
    RenderLayoutType layout_type;
    float2 viewport_size;
} RenderManagerConfig;

typedef struct {
    RenderViewport viewport;
    float4x4 view_matrix;
} RenderManager;

extern RenderManager gRenderManager;

void render_manager_init(RenderManagerConfig *config);
void render_manager_destroy(void);
void render_manager_size_changed(GpuView *view);

#endif // RenderManager_h
