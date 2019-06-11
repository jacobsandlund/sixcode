#ifndef OsApplication_h
#define OsApplication_h

#include "spacetime.h"
#include "os/window.h"

typedef void (*OsApplicationNotificationFn)(void);

typedef struct {
    OsApplicationNotificationFn will_terminate;
} OsApplicationConfig;

typedef struct {
    void *application_impl;
    OsApplicationNotificationFn will_terminate;
} OsApplication;

void os_application_init(OsApplication *app, OsApplicationConfig *config);
void os_application_destroy(OsApplication *app);
void os_application_finish_launching(OsApplication *app);

#endif // OsApplication_h
