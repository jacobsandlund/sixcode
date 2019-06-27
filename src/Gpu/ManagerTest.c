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
#include "Gpu/Renderer.c"
#include "Gpu/ViewMock.c"

#include "Test.h"

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
            .draw_in_view = GpuManagerDrawInView,
            .size_changed = GpuManagerSizeChanged,
        },
    };
    OsScreenFrame visible_frame = {
        .origin = { 0, 0 },
        .size = { 640, 480 },
    };

    GpuManagerInit(visible_frame, &config);

    _d(gGpuManager.renderer.pipeline_state->device == gGpuManager.device);
    //=> 1

    GpuManagerRegisterCallbacks(&callbacks);

    _d(gGpuManager.view->draw_in_view == GpuManagerDrawInView);
    //=> 1

    GpuViewMockDrawInView(gGpuManager.view);

    _d(gGpuCommandEncoderMock.draw_vertex_count);
    //=> 2250

    GpuManagerDestroy();
}
