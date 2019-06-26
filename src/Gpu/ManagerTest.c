#include "Gpu/Manager.c"

#include "Gpu/DeviceMock.c"
#include "Gpu/Renderer.c"
#include "Gpu/ViewMock.c"

#include "Test.h"

Test(gpu_manager)
{
    GpuManagerConfig config = {
        .view = {
            .preferred_frames_per_second = 60,
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

    _d(gGpuManager.renderer->device == gGpuManager.device);
    //=> 1

    GpuManagerRegisterCallbacks(&callbacks);

    _d(gGpuManager.view->draw_in_view == GpuManagerDrawInView);
    //=> 1

    GpuManagerDrawInView(gGpuManager.view);

    _d(gGpuManager.renderer->drew_in_view == gGpuManager.view);
    //=> 1

    GpuManagerDestroy();
}
