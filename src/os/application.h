#ifndef OsApplication_h
#define OsApplication_h

#include "spacetime.h"
#include "os/window.h"

typedef void (*OsApplicationNotificationFn)(void);

typedef struct {
    OsApplicationNotificationFn will_terminate;
} OsApplicationConfig;

typedef struct OsApplication OsApplication;

OsApplication *os_application_create(OsApplicationConfig *config);
void os_application_destroy(OsApplication *app);
void os_application_finish_launching(OsApplication *app);

#endif // OsApplication_h
