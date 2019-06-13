#include "os/application.h"
#include "test/allocator.h"

struct OsApplication {
    bool finished_launching;
    OsApplicationNotificationFn will_terminate;
};

void os_application_mock_terminate(OsApplication *app)
{
    app->will_terminate();
}

OsApplication *os_application_create(OsApplicationConfig *config)
{
    OsApplication *app = tmalloc(sizeof *app);
    app->finished_launching = false;
    app->will_terminate = config->will_terminate;
    return app;
}

void os_application_destroy(OsApplication *app)
{
    tfree(app);
}

void os_application_finish_launching(OsApplication *app)
{
    app->finished_launching = true;
}
