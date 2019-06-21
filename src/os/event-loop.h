#ifndef _OsEventLoop_h
#define _OsEventLoop_h

#include "spacetime.h"
#include "os/event-queue.h"

typedef struct OsEventLoop OsEventLoop;

OsEventLoop *OsEventLoopCreate(void);
void OsEventLoopDestroy(OsEventLoop *loop);
void OsEventLoopRun(OsEventLoop *loop, OsEventQueue *queue);

#endif // _OsEventLoop_h
