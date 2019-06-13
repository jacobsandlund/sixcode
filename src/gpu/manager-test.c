#include "gpu/manager.c"
#include "gpu/view-mock.c"
#include "gpu/device-mock.c"
#include "gpu/renderer.c"
#include "test.h"

Test(gpu_manager)
{
    GpuManagerConfig config = {
        .view = {
            .preferred_frames_per_second = 60,
        },
    };
    GpuManagerCallbacks callbacks = {
        .view = {
            .draw_in_view = gpu_manager_draw_in_view,
            .size_changed = gpu_manager_size_changed,
        }
    };
    OsScreenFrame visible_frame = {
        .origin = {0, 0},
        .size = {640, 480},
    };

    gpu_manager_init(visible_frame, &config);

    _d(gGpuManager.renderer->device == gGpuManager.device);
    //=> 1

    gpu_manager_register_callbacks(&callbacks);

    _d(gGpuManager.view->draw_in_view == gpu_manager_draw_in_view);
    //=> 1

    gpu_manager_draw_in_view(gGpuManager.view);

    _d(gGpuManager.renderer->drew_in_view == gGpuManager.view);
    //=> 1

    gpu_manager_destroy();
}
