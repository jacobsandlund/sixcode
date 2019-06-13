#include "gpu/manager.c"
#include "gpu/view-mock.c"
#include "gpu/device-mock.c"
#include "gpu/renderer-mock.c"
#include "test.h"

bool test_draw_in_view_called;

void test_size_changed(GpuView *view, float2 viewport_size)
{
    (void) view;
    (void) viewport_size;
}

void test_draw_in_view(GpuView *view)
{
    (void) view;
    test_draw_in_view_called = true;
}

Test(gpu_manager)
{
    GpuManagerConfig config = {
        .view = {
            .draw_in_view = test_draw_in_view,
            .size_changed = test_size_changed,
        },
    };
    OsScreenFrame visible_frame = {
        .origin = {0, 0},
        .size = {640, 480},
    };

    gpu_manager_init(visible_frame, &config);

    _d(gGpuManager.view->draw_in_view == test_draw_in_view);
    //=> 1
    _d(gGpuManager.renderer->device == gGpuManager.device);
    //=> 1

    gpu_manager_draw_in_view(gGpuManager.view);

    gpu_manager_destroy();
}
