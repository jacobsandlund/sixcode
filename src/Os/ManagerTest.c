#include "Os/Manager.c"
#include "Os/ApplicationMock.c"
#include "Os/ClockMock.c"
#include "Os/ScreenMock.c"
#include "Os/EventLoopMock.c"
#include "Os/EventQueue.c"
#include "Os/WindowMock.c"
#include "Gpu/ViewMock.c"
#include "Log/ManagerMock.c"
#include "Test.h"

bool test_application_will_terminate_called = false;

void test_application_will_terminate(void)
{
    test_application_will_terminate_called = true;
}

static OsManagerCallbacks test_manager_callbacks = {
    .application = {
        .will_terminate = test_application_will_terminate,
    },
};

static OsManagerConfig test_manager_config = {
    .event_queue_length = 32,
    .event_queue_safe_length_remaining = 8,
};

Test(OsManagerInit)
{
    OsManagerInit(&test_manager_config);
    OsManagerRegisterCallbacks(&test_manager_callbacks);

    _d(gOsManager.event_queue.length);
    //=> 32
    _d(gOsManager.event_queue.safe_read_behind);
    //=> 24
    _d(gOsManager.application->will_terminate == test_application_will_terminate);
    //=> 1

    OsManagerDestroy();
}

Test(os_manager_window_init_and_finish_launching)
{
    float2 screen_size = {2560, 1440};
    os_screen_mock_init(screen_size);
    OsScreenFrame frame = OsScreenVisibleFrame();
    GpuView *view = GpuViewCreate(NULL, frame, &gGpuViewMockConfig);

    OsManagerInit(&test_manager_config);
    OsManagerRegisterCallbacks(&test_manager_callbacks);

    OsManagerWindowInit(view);

    _d(gOsManager.window->view == view);
    //=> 1
    _d(gOsManager.window->showing);
    //=> 0

    OsManagerFinishLaunching();

    _d(gOsManager.application->finished_launching);
    //=> 1
    _d(gOsManager.window->showing);
    //=> 1

    OsManagerWindowDestroy();
    GpuViewDestroy(view);
    OsManagerDestroy();
}

Test(OsManagerEventLoopRun)
{
    os_clock_mock_init(12345, 321);
    LogManagerInit(&gLogManagerMockConfig);
    OsManagerInit(&test_manager_config);
    OsManagerRegisterCallbacks(&test_manager_callbacks);

    OsEventLoop *loop = gOsManager.event_loop;
    float2 location = {1300, 400};
    os_event_loop_mock_next_event_location(loop, location);
    os_event_loop_mock_event_type(loop, OsEventTypeMouseMove);
    os_event_loop_mock_event_type(loop, OsEventTypeMouseClick);
    os_event_loop_mock_event_type(loop, OsEventTypeTerminateLoop);

    OsManagerEventLoopRun();

    OsEventQueue *queue = &gOsManager.event_queue;
    OsEvent event;

    _d(OsEventQueueRead(queue, &event));
    //=> 1
    _d(event.type == OsEventTypeMouseMove);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 12345

    _d(OsEventQueueRead(queue, &event));
    //=> 2
    _d(event.type == OsEventTypeMouseClick);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 12666

    OsManagerDestroy();
    log_manager_destroy();
}
