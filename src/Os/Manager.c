#include "Os/Manager.h"

OsManager gOsManager;

void OsManagerInit(OsManagerConfig *config)
{
    OsEventQueueInit(&gOsManager.event_queue,
                     config->event_queue_length,
                     config->event_queue_safe_length_remaining);

    gOsManager.application = OsApplicationCreate();

    gOsManager.event_loop = OsEventLoopCreate();
}

void OsManagerDestroy(void)
{
    OsEventLoopDestroy(gOsManager.event_loop);
    OsApplicationDestroy(gOsManager.application);
    OsEventQueueDestroy(&gOsManager.event_queue);
}

void OsManagerRegisterCallbacks(OsManagerCallbacks *callbacks)
{
    OsApplicationRegisterCallbacks(gOsManager.application,
                                   &callbacks->application);
}

void OsManagerWindowInit(GpuView *view)
{
    gOsManager.window = OsWindowCreate(view);
}

void OsManagerWindowDestroy()
{
    OsWindowDestroy(gOsManager.window);
}

void OsManagerFinishLaunching(void)
{
    OsApplicationFinishLaunching(gOsManager.application);
    OsWindowShow(gOsManager.window);
}

void OsManagerEventLoopRun(void)
{
    OsEventLoopRun(gOsManager.event_loop, &gOsManager.event_queue);
}
