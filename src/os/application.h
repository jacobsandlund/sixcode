#ifndef OsApplication_h
#define OsApplication_h

#include "spacetime.h"

typedef void (*OsApplicationNotificationFn)(void);

typedef struct {
    OsApplicationNotificationFn will_terminate;
} OsApplicationCallbacks;

typedef struct OsApplication OsApplication;

OsApplication *os_application_create(void);
void os_application_destroy(OsApplication *app);
void os_application_register_callbacks(OsApplication *app, OsApplicationCallbacks *callbacks);
void os_application_finish_launching(OsApplication *app);

#endif // OsApplication_h
