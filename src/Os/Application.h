#ifndef _OsApplication_h
#define _OsApplication_h

#include "Spacetime.h"

typedef void (*OsApplicationNotificationFn)(void);

typedef struct {
    OsApplicationNotificationFn will_terminate;
} OsApplicationCallbacks;

typedef struct OsApplication OsApplication;

OsApplication *OsApplicationCreate(void);
void OsApplicationDestroy(OsApplication *app);
void OsApplicationRegisterCallbacks(OsApplication *app,
                                    OsApplicationCallbacks *callbacks);
void OsApplicationFinishLaunching(OsApplication *app);

#endif  // _OsApplication_h
