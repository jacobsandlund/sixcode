#include "Gpu/Manager.c"

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

void TestDrawInView(GpuView *view)
{
    (void)view;
}

void TestSizeChanged(GpuView *view, float2 viewport_size)
{
    (void)view;
    (void)viewport_size;
}

Test(gpu_manager)
{
    GpuManagerConfig config = {
        .view = {
            .preferred_frames_per_second = 60,
            .color_pixel_format = GpuPixelFormatBGRA8Unorm_sRGB,
        },
    };
    GpuManagerCallbacks callbacks = {
        .view = {
            .draw_in_view = TestDrawInView,
            .size_changed = TestSizeChanged,
        },
    };
    OsScreenFrame visible_frame = {
        .origin = { 0, 0 },
        .size = { 640, 480 },
    };

    GpuManagerInit(visible_frame, &config);

    _d(gGpuManager.view->device == gGpuManager.device);
    //=> 1
    _d(gGpuManager.command_queue->device == gGpuManager.device);
    //=> 1

    GpuManagerRegisterCallbacks(&callbacks);

    _d(gGpuManager.view->draw_in_view == TestDrawInView);
    //=> 1
    _d(gGpuManager.view->size_changed == TestSizeChanged);
    //=> 1

    GpuManagerDestroy();
}
