#include "engine/manager.c"

// From: bin/ls manager-test.c | grep -v 'engine/manager-test.c' | xargs grep -h '#include' | grep -v test/test.h | sort -u
#include "camera/camera.c"
#include "camera/manager.c"
#include "gpu/device-mock.c"
#include "gpu/manager.c"
#include "gpu/renderer-mock.c"
#include "gpu/view-mock.c"
#include "log/manager-mock.c"
#include "math/fnv.c"
#include "math/quad.c"
#include "os/application-mock.c"
#include "os/clock-mock.c"
#include "os/event-loop-mock.c"
#include "os/event-queue.c"
#include "os/manager.c"
#include "os/screen-mock.c"
#include "os/window-mock.c"
#include "render/layout.c"
#include "render/manager.c"
#include "render/vector.c"
#include "resource/manager.c"
#include "resource/pointer-allocator.c"
#include "resource/resource.c"
#include "string/id-table.c"
#include "string/id.c"
#include "string/manager.c"
#include "world/grid.c"
#include "world/manager.c"
#include "world/vector.c"

#include "test/test.h"

Test(engine_manager)
{
    float2 screen_size = {1920, 1080};
    os_screen_mock_init(screen_size);
    os_clock_mock_init(12345, 321);

    // Save some time, especially for valgrind
    gEngineConfig.world_grid_random_count = 100;

    engine_manager_init(&gEngineConfig);

    _f2(gRenderManager.viewport.size);
    //=> 1920, 1080

    GpuView *view = gGpuManager.view;
    float2 viewport_size = {2560, 1440};
    gpu_view_mock_size_changed(view, viewport_size);

    _f2(gRenderManager.viewport.size);
    //=> 2560, 1440

    gpu_view_mock_draw_in_view(view);

    _d(gGpuManager.renderer->drew_in_view == view);
    //=> 1

    OsEventLoop *loop = gOsManager.event_loop;
    float2 location = {1300, 400};
    os_event_loop_mock_next_event_location(loop, location);
    os_event_loop_mock_event_type(loop, OsEventTypeMouseMove);
    os_event_loop_mock_event_type(loop, OsEventTypeMouseClick);
    os_event_loop_mock_event_type(loop, OsEventTypeTerminateLoop);

    engine_manager_run();

    OsEventQueue *queue = &gOsManager.event_queue;
    OsEvent event;

    _d(os_event_queue_read(queue, &event));
    //=> 1
    _d(event.type == OsEventTypeMouseMove);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 17160

    _d(os_event_queue_read(queue, &event));
    //=> 2
    _d(event.type == OsEventTypeMouseClick);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 17481

    os_application_mock_terminate(gOsManager.application);

    _d(test_allocator_was_freed(gOsManager.application));
    //=> 1
    _d(test_allocator_was_freed(gGpuManager.view));
    //=> 1
}
