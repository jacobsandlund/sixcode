#include "os/manager.h"

OsManager gOsManager;

void os_manager_init(OsManagerConfig *config)
{
    os_event_queue_init(&gOsManager.event_queue,
            config->event_queue_length,
            config->event_queue_safe_length_remaining);

    gOsManager.application = os_application_create(&config->application);

    gOsManager.event_loop = os_event_loop_create();
}

void os_manager_destroy(void)
{
    os_event_loop_destroy(gOsManager.event_loop);
    os_application_destroy(gOsManager.application);
    os_event_queue_destroy(&gOsManager.event_queue);
}

void os_manager_window_init(GpuView *view)
{
    gOsManager.window = os_window_create(view);
}

void os_manager_window_destroy()
{
    os_window_destroy(gOsManager.window);
}

void os_manager_finish_launching(void)
{
    os_application_finish_launching(gOsManager.application);
    os_window_show(gOsManager.window);
}

void os_manager_event_loop_run(void)
{
    os_event_loop_run(gOsManager.event_loop, &gOsManager.event_queue);
}
