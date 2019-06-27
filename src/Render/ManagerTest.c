#include "Render/Manager.c"

#include "Gpu/BufferMock.c"
#include "Gpu/CmdMock.c"
#include "Gpu/CommandBufferMock.c"
#include "Gpu/CommandEncoderMock.c"
#include "Gpu/CommandQueueMock.c"
#include "Gpu/DeviceMock.c"
#include "Gpu/FunctionMock.c"
#include "Gpu/Manager.c"
#include "Gpu/PipelineStateMock.c"
#include "Gpu/RenderPassConfigMock.c"
#include "Gpu/ViewMock.c"
#include "Render/Layout.c"
#include "Render/Renderer.c"

#include "Test.h"

Test(RenderManager)
{
    GpuViewConfig view_config = {
        .preferred_frames_per_second = 60,
        .color_pixel_format = GpuPixelFormatBGRA8Unorm_sRGB,
    };
    GpuManagerConfig gpu_config = {
        .view = view_config,
    };
    OsScreenFrame visible_frame = {
        .origin = { 0, 0 },
        .size = { 640, 480 },
    };
    GpuManagerCallbacks gpu_callbacks = {
        .view = {
            .draw_in_view = RenderManagerDrawInView,
            .size_changed = RenderManagerSizeChanged,
        },
    };

    RenderManagerConfig config = {
        .layout_type = RenderLayoutTypeRect,
        .renderer = {
            .view = view_config,
        },
    };

    GpuManagerInit(visible_frame, &gpu_config);

    RenderManagerInit(&config);

    _d(gRenderManager.viewport.layout.type == RenderLayoutTypeRect);
    //=> 1
    _f2(gRenderManager.viewport.size);
    //=> 640, 480

    GpuManagerRegisterCallbacks(&gpu_callbacks);

    float2 new_viewport_size = { 2560, 1440 };

    GpuViewMockSizeChanged(gGpuManager.view, new_viewport_size);

    _f2(gRenderManager.viewport.size);
    //=> 2560, 1440

    GpuViewMockDrawInView(gGpuManager.view);

    _d(gGpuCommandEncoderMock.draw_vertex_count);
    //=> 2250

    RenderManagerDestroy();
    GpuManagerDestroy();
}
