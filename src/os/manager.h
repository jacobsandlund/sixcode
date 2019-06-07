#ifndef OsManager_h
#define OsManager_h

#include "spacetime.h"
#include "os/event-queue.h"
#include "os/event-loop.h"
#include "os/application.h"
#include "os/window.h"
#include "gpu/view.h"

typedef struct {
    OsApplicationConfig application;
    i64 event_queue_length;
    i64 event_queue_safe_length_remaining;
} OsManagerConfig;

typedef struct {
    OsEventQueue event_queue;
    OsEventLoop event_loop;
    OsApplication application;
    OsWindow window;
} OsManager;

extern OsManager gOsManager;

void os_manager_init(OsManagerConfig *config);
void os_manager_destroy(void);
void os_manager_window_set_view(GpuView *view);
void os_manager_finish_launching(void);
void os_manager_run_event_loop(void);

#endif // OsManager_h
