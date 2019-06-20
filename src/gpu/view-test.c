#include "gpu/view-mock.c"
#include "gpu/device-mock.c"
#include "test.h"

float2 test_view_size;
bool test_draw_in_view_called;

void test_size_changed(GpuView *view, float2 viewport_size)
{
    (void) view;
    test_view_size = viewport_size;
}

void test_draw_in_view(GpuView *view)
{
    (void) view;
    test_draw_in_view_called = true;
}

Test(gpu_view)
{
    float2 old_viewport_size = {1920, 1080};
    float2 new_viewport_size = {2560, 1920};
    GpuViewConfig config = {
        .preferred_frames_per_second = 60,
        .color_pixel_format = 2,
    };
    OsScreenFrame visible_frame = {
        .origin = {0, 0},
        .size = old_viewport_size,
    };
    GpuDevice *device = gpu_device_create();
    GpuView *view = gpu_view_create(device, visible_frame, &config);

    _d(view->device == device);
    //=> 1
    _d(view->preferred_frames_per_second);
    //=> 60
    _d(view->color_pixel_format);
    //=> 2
    _f2(gpu_view_viewport_size(view));
    //=> 1920, 1080
    _d(gpu_view_color_pixel_format(view));
    //=> 2

    GpuViewCallbacks callbacks = {
        .draw_in_view = test_draw_in_view,
        .size_changed = test_size_changed,
    };
    gpu_view_register_callbacks(view, &callbacks);

    gpu_view_mock_size_changed(view, new_viewport_size);

    _f2(gpu_view_viewport_size(view));
    //=> 2560, 1920
    _f2(test_view_size);
    //=> 2560, 1920

    gpu_view_mock_draw_in_view(view);

    _d(test_draw_in_view_called);
    //=> 1

    _d(gpu_view_current_render_pass_config(view) != NULL);
    //=> 1
    view->has_current_render_pass = false;
    _d(gpu_view_current_render_pass_config(view) != NULL);
    //=> 0

    gpu_view_destroy(view);
    gpu_device_destroy(device);
}

Test(gpu_view_mock_config)
{
    OsScreenFrame frame = {};
    GpuView *view = gpu_view_create(NULL, frame, &gGpuViewMockConfig);

    // Doesn't blow up

    gpu_view_mock_draw_in_view(view);
    float2 size = {3, 4};
    gpu_view_mock_size_changed(view, size);

    gpu_view_destroy(view);
}
