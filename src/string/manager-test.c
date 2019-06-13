#include "log/manager-mock.c"
#include "math/fnv.c"
#include "string/id-table.c"
#include "string/id.c"
#include "string/manager.c"
#include "test.h"

Test(string_manager_init)
{
    StringManagerConfig config = {
        .string_table_size = 4,
    };
    log_manager_init(&gLogManagerMockConfig);
    string_manager_init(&config);

    StringIdTable *string_table = &gStringManager.string_table;

    _d(string_table->size);
    //=> 4
    _d(string_table->sizeof_value);
    //=> 8

    _u64(StringIdIntern("foo"));
    //=> 15902901984413996407

    _s(*((u8 **) string_id_table_get(string_table, StringId("foo"))));
    //=> foo

    _s(StringIdString(StringId("foo")));
    //=> foo

    string_manager_destroy();
    log_manager_destroy();
}
