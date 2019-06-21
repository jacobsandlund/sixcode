#ifndef _OsManager_h
#define _OsManager_h

#include "Spacetime.h"
#include "Os/EventQueue.h"
#include "Os/EventLoop.h"
#include "Os/Application.h"
#include "Os/Window.h"
#include "Gpu/view.h"

typedef struct {
    i64 event_queue_length;
    i64 event_queue_safe_length_remaining;
} OsManagerConfig;

typedef struct {
    OsApplicationCallbacks application;
} OsManagerCallbacks;

typedef struct {
    OsEventQueue event_queue;
    OsEventLoop *event_loop;
    OsApplication *application;
    OsWindow *window;
} OsManager;

extern OsManager gOsManager;

void OsManagerInit(OsManagerConfig *config);
void OsManagerDestroy(void);
void OsManagerRegisterCallbacks(OsManagerCallbacks *callbacks);
void OsManagerWindowInit(GpuView *view);
void OsManagerWindowDestroy(void);
void OsManagerFinishLaunching(void);
void OsManagerEventLoopRun(void);

#endif // _OsManager_h
