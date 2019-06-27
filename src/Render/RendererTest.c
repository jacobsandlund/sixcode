#include "Render/Renderer.c"

#include "Gpu/BufferMock.c"
#include "Gpu/CmdMock.c"
#include "Gpu/CommandBufferMock.c"
#include "Gpu/CommandEncoderMock.c"
#include "Gpu/CommandQueueMock.c"
#include "Gpu/DeviceMock.c"
#include "Gpu/FunctionMock.c"
#include "Gpu/PipelineStateMock.c"
#include "Gpu/RenderPassConfigMock.c"
#include "Gpu/ViewMock.c"
#include "Render/Layout.c"

#include "Test.h"

RenderViewport viewport;
Renderer renderer;
GpuCommandQueue *queue;

void TestRendererDrawInViewFn(GpuView *view)
{
    RendererDrawInView(&renderer, view, &viewport, queue);
}

void TestRendererSizeChangedFn(GpuView *view, float2 viewport_size)
{
    (void)view;
    viewport.size = viewport_size;
}

GpuViewCallbacks test_callbacks = {
    .draw_in_view = TestRendererDrawInViewFn,
    .size_changed = TestRendererSizeChangedFn,
};

Test(gpu_renderer)
{
    OsScreenFrame frame = {
        .origin = { 0, 0 },
        .size = { 640, 480 },
    };

    GpuDevice *device = GpuDeviceCreate();
    GpuView *view = GpuViewCreate(device, frame, &gGpuViewMockConfig);

    queue = GpuCommandQueueCreate(device);

    RendererConfig config = {
        .view = {
            .color_pixel_format = GpuPixelFormatBGRA8Unorm_sRGB,
        },
    };

    RenderViewportLayout(&viewport, RenderLayoutTypeHex);
    viewport.size = GpuViewViewportSize(view);

    RendererInit(&renderer, device, &config);

    GpuViewRegisterCallbacks(view, &test_callbacks);

    GpuViewMockDrawInView(view);

    _d(gGpuCommandEncoderMock.draw_vertex_count);
    //=> 2250

    RendererDestroy(&renderer);

    GpuCommandQueueDestroy(queue);
    GpuViewDestroy(view);
    GpuDeviceDestroy(device);
}
