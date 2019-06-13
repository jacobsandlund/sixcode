#include "os/application-mock.c"
#include "test.h"

static bool called_will_terminate = false;

void test_will_terminate(void)
{
    called_will_terminate = true;
}

Test(os_application)
{
    OsApplicationCallbacks callbacks = {
        .will_terminate = test_will_terminate,
    };
    called_will_terminate = false;

    OsApplication *app = os_application_create();
    os_application_register_callbacks(app, &callbacks);

    _d(app->finished_launching);
    //=> 0

    os_application_finish_launching(app);

    _d(app->finished_launching);
    //=> 1

    os_application_mock_terminate(app);

    _d(called_will_terminate);
    //=> 1

    os_application_destroy(app);
}
