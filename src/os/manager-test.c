#include "os/manager.c"
#include "os/application-mock.c"
#include "os/clock-mock.c"
#include "os/screen-mock.c"
#include "os/event-loop-mock.c"
#include "os/event-queue.c"
#include "os/window-mock.c"
#include "gpu/view-mock.c"
#include "log/manager-mock.c"
#include "test.h"

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

Test(os_manager_init)
{
    os_manager_init(&test_manager_config);
    os_manager_register_callbacks(&test_manager_callbacks);

    _d(gOsManager.event_queue.length);
    //=> 32
    _d(gOsManager.event_queue.safe_read_behind);
    //=> 24
    _d(gOsManager.application->will_terminate == test_application_will_terminate);
    //=> 1

    os_manager_destroy();
}

Test(os_manager_window_init_and_finish_launching)
{
    float2 screen_size = {2560, 1440};
    os_screen_mock_init(screen_size);
    OsScreenFrame frame = os_screen_visible_frame();
    GpuView *view = gpu_view_create(NULL, frame, &gGpuViewMockConfig);

    os_manager_init(&test_manager_config);
    os_manager_register_callbacks(&test_manager_callbacks);

    os_manager_window_init(view);

    _d(gOsManager.window->view == view);
    //=> 1
    _d(gOsManager.window->showing);
    //=> 0

    os_manager_finish_launching();

    _d(gOsManager.application->finished_launching);
    //=> 1
    _d(gOsManager.window->showing);
    //=> 1

    os_manager_window_destroy();
    gpu_view_destroy(view);
    os_manager_destroy();
}

Test(os_manager_event_loop_run)
{
    os_clock_mock_init(12345, 321);
    log_manager_init(&gLogManagerMockConfig);
    os_manager_init(&test_manager_config);
    os_manager_register_callbacks(&test_manager_callbacks);

    OsEventLoop *loop = gOsManager.event_loop;
    float2 location = {1300, 400};
    os_event_loop_mock_next_event_location(loop, location);
    os_event_loop_mock_event_type(loop, OsEventTypeMouseMove);
    os_event_loop_mock_event_type(loop, OsEventTypeMouseClick);
    os_event_loop_mock_event_type(loop, OsEventTypeTerminateLoop);

    os_manager_event_loop_run();

    OsEventQueue *queue = &gOsManager.event_queue;
    OsEvent event;

    _d(os_event_queue_read(queue, &event));
    //=> 1
    _d(event.type == OsEventTypeMouseMove);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 12345

    _d(os_event_queue_read(queue, &event));
    //=> 2
    _d(event.type == OsEventTypeMouseClick);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 12666

    os_manager_destroy();
    log_manager_destroy();
}
