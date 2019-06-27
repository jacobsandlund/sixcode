#include "Engine/Manager.c"

// From:
// st ls ManagerTest.c | grep -v 'Engine/ManagerTest\.c' |
// xargs grep -h '#include' | grep -v 'Test\.h' | sort -u

#include "Camera/Camera.c"
#include "Camera/Manager.c"
#include "Gpu/BufferMock.c"
#include "Gpu/CmdMock.c"
#include "Gpu/CommandBufferMock.c"
#include "Gpu/CommandEncoderMock.c"
#include "Gpu/CommandQueueMock.c"
#include "Gpu/DeviceMock.c"
#include "Gpu/FunctionMock.c"
#include "Gpu/Manager.c"
#include "Gpu/PipelineStateMock.c"
#include "Gpu/RenderPassConfigMock.c"
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

Test(EngineManager)
{
    float2 screen_size = { 1920, 1080 };
    os_screen_mock_init(screen_size);
    os_clock_mock_init(12345, 321);

    // Save some time, especially for valgrind
    gEngineManagerConfig.world_grid_random_count = 100;

    EngineManagerInit(&gEngineManagerCallbacks, &gEngineManagerConfig);

    _f2(gRenderManager.viewport.size);
    //=> 0, 0

    GpuView *view = gGpuManager.view;
    float2 viewport_size = { 2560, 1440 };
    GpuViewMockSizeChanged(view, viewport_size);

    _f2(gRenderManager.viewport.size);
    //=> 0, 0

    GpuViewMockDrawInView(view);

    _d(gGpuCommandEncoderMock.draw_vertex_count);
    //=> 2250

    OsEventLoop *loop = gOsManager.event_loop;
    float2 location = { 1300, 400 };
    os_event_loop_mock_next_event_location(loop, location);
    os_event_loop_mock_event_type(loop, OsEventTypeMouseMove);
    os_event_loop_mock_event_type(loop, OsEventTypeMouseClick);
    os_event_loop_mock_event_type(loop, OsEventTypeTerminateLoop);

    EngineManagerRun();

    OsEventQueue *queue = &gOsManager.event_queue;
    OsEvent event;

    _d(OsEventQueueRead(queue, &event));
    //=> 1
    _d(event.type == OsEventTypeMouseMove);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 17160

    _d(OsEventQueueRead(queue, &event));
    //=> 2
    _d(event.type == OsEventTypeMouseClick);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 17481

    os_application_mock_terminate(gOsManager.application);
}
