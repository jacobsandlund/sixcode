#include "Gpu/ViewMock.c"
#include "Os/WindowMock.c"

#include "Test.h"

Test(os_window)
{
    OsScreenFrame frame = {};
    GpuView *view = GpuViewCreate(NULL, frame, &gGpuViewMockConfig);

    OsWindow *window = OsWindowCreate(view);

    _d(window->view == view);
    //=> 1
    _d(window->showing);
    //=> 0

    OsWindowShow(window);

    _d(window->showing);
    //=> 1

    OsWindowDestroy(window);
    GpuViewDestroy(view);
}
