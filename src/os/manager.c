#include "os/manager.h"

OsManager gOsManager;

void os_manager_init(OsManagerConfig *config)
{
    os_event_queue_init(&gOsManager.event_queue,
            config->event_queue_length,
            config->event_queue_safe_length_remaining);

    os_application_init(&gOsManager.application);
}

void os_manager_destroy(void)
{
    os_application_destroy(&gOsManager.application);
    os_event_queue_destroy(&gOsManager.event_queue);
}

void os_manager_window_set_view(GpuView *view)
{
    os_window_set_view(&gOsManager.window, view);
}

void os_manager_finish_launching(void)
{
    os_window_show(&gOsManager.window);
    os_application_finish_launching(&gOsManager.application, &gOsManager.window);
}

void os_manager_run_event_loop(void)
{
    os_event_loop_run(&gOsManager.event_loop, &gOsManager.event_queue);
}
