#include "os/window-mock.c"
#include "gpu/view-mock.c"
#include "test/test.h"

Test(os_window)
{
    OsScreenFrame frame = {};
    GpuView *view = gpu_view_create(NULL, frame, &gGpuViewMockConfig);

    OsWindow *window = os_window_create(view);

    _d(window->view == view);
    //=> 1
    _d(window->showing);
    //=> 0

    os_window_show(window);

    _d(window->showing);
    //=> 1

    os_window_destroy(window);

    _d(test_allocator_was_freed(window));
    //=> 1
}
