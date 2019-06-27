#include "Gpu/Renderer.c"

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

#include "Test.h"

GpuRenderer renderer;
GpuCommandQueue *queue;

void TestGpuRendererDrawInViewFn(GpuView *view)
{
    GpuRendererDrawInView(&renderer, view, queue);
}

void TestGpuRendererSizeChangedFn(GpuView *view, float2 viewport_size)
{
    GpuRendererSizeChanged(&renderer, view, viewport_size);
}

GpuViewCallbacks test_callbacks = {
    .draw_in_view = TestGpuRendererDrawInViewFn,
    .size_changed = TestGpuRendererSizeChangedFn,
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

    GpuRendererConfig config = {
        .viewport_size = GpuViewViewportSize(view),
        .pixel_format = GpuPixelFormatBGRA8Unorm_sRGB,
    };
    GpuRendererInit(&renderer, device, &config);

    GpuViewRegisterCallbacks(view, &test_callbacks);

    GpuViewMockDrawInView(view);

    _d(gGpuCommandEncoderMock.draw_vertex_count);
    //=> 2250

    GpuRendererDestroy(&renderer);

    GpuCommandQueueDestroy(queue);
    GpuViewDestroy(view);
    GpuDeviceDestroy(device);
}
