#include "Os/Application.h"
#include <stdlib.h>

struct OsApplication {
    bool finished_launching;
    OsApplicationNotificationFn will_terminate;
};

void os_application_mock_terminate(OsApplication *app)
{
    app->will_terminate();
}

OsApplication *OsApplicationCreate(void)
{
    OsApplication *app = malloc(sizeof *app);
    app->finished_launching = false;
    return app;
}

void OsApplicationDestroy(OsApplication *app)
{
    free(app);
}

void OsApplicationRegisterCallbacks(OsApplication *app, OsApplicationCallbacks *callbacks)
{
    app->will_terminate = callbacks->will_terminate;
}

void OsApplicationFinishLaunching(OsApplication *app)
{
    app->finished_launching = true;
}
