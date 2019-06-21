#include "Engine/Manager.c"

// From: bin/ls ManagerTest.c | grep -v 'Engine/ManagerTest\.c' | xargs grep -h '#include' | grep -v 'Test\.h' | sort -u
#include "Camera/Camera.c"
#include "Camera/Manager.c"
#include "Gpu/DeviceMock.c"
#include "Gpu/Manager.c"
#include "Gpu/Renderer.c"
#include "Gpu/ViewMock.c"
#include "Log/ManagerMock.c"
#include "Math/Quad.c"
#include "Os/ApplicationMock.c"
#include "Os/ClockMock.c"
#include "Os/EventLoopMock.c"
#include "Os/EventQueue.c"
#include "Os/Manager.c"
#include "Os/ScreenMock.c"
#include "Os/WindowMock.c"
#include "Render/Layout.c"
#include "Render/Manager.c"
#include "Render/Vector.c"
#include "World/Grid.c"
#include "World/Manager.c"
#include "World/Vector.c"

#include "Test.h"

Test(engine_manager)
{
    float2 screen_size = {1920, 1080};
    os_screen_mock_init(screen_size);
    os_clock_mock_init(12345, 321);

    // Save some time, especially for valgrind
    gEngineManagerConfig.world_grid_random_count = 100;

    EngineManagerInit(&gEngineManagerCallbacks, &gEngineManagerConfig);

    _f2(gRenderManager.viewport.size);
    //=> 0, 0

    GpuView *view = gGpuManager.view;
    float2 viewport_size = {2560, 1440};
    gpu_view_mock_size_changed(view, viewport_size);

    _f2(gRenderManager.viewport.size);
    //=> 0, 0

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

    _d(OsEventQueueRead(queue, &event));
    //=> 1
    _d(event.type == OsEventTypeMouseMove);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 17802

    _d(OsEventQueueRead(queue, &event));
    //=> 2
    _d(event.type == OsEventTypeMouseClick);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 18123

    os_application_mock_terminate(gOsManager.application);
}
