#include "string/id.c"
#include "log/manager-mock.c"
#include "math/fnv.c"
#include "string/id-table.c"
#include <string.h>
#include "test.h"

Test(string_id)
{
    u8 s1[] = "foo";
    _u64(string_id((const u8 *) s1, strlen((char *) s1)));
    //=> 15902901984413996407

    u8 s2[] = "The quick brown fox jumps over the lazy dog";
    _u64(string_id((const u8 *) s2, strlen((char *) s2)));
    //=> 17580284887202820368

    // Literal

    _u64(string_id_literal("foo"));
    //=> 15902901984413996407
    _u64(string_id_literal("foobar"));
    //=> 9625390261332436968
    _u64(string_id_literal("foobas"));
    //=> 9625391360844065179
}

Test(string_id_intern)
{
    StringIdTable *st = tmalloc(sizeof *st);

    log_manager_init(&gLogManagerMockConfig);
    string_id_table_init(st, 4, sizeof(u8 *));

    _d(st->count);
    //=> 0

    u8 s1[] = "foo";
    _u64(string_id_intern(st, (const u8 *) s1, strlen((char *) s1)));
    //=> 15902901984413996407
    _d(st->count);
    //=> 1

    u8 s2[] = "The quick brown fox jumps over the lazy dog";
    _u64(string_id_intern(st, (const u8 *) s2, strlen((char *) s2)));
    //=> 17580284887202820368
    _d(st->count);
    //=> 2

    // Literal

    // And with the same string
    _u64(string_id_intern_literal(st, "foo"));
    //=> 15902901984413996407
    _d(st->count);  // Same count
    //=> 2

    _u64(string_id_intern_literal(st, "foobar"));
    //=> 9625390261332436968
    _u64(string_id_intern_literal(st, "foobas"));
    //=> 9625391360844065179

    _d(st->count);
    //=> 4

    string_id_table_destroy(st);
}

Test(string_id_string)
{
    StringIdTable *st = tmalloc(sizeof *st);

    log_manager_init(&gLogManagerMockConfig);
    string_id_table_init(st, 4, sizeof(u8 *));

    sid id = string_id_intern_literal(st, "foobar");
    _u64(id);
    //=> 9625390261332436968

    _s(string_id_string(st, id));
    //=> foobar

    _s(string_id_string(st, string_id_literal("this won't be found")));
    //=> !!! Not found !!!

    string_id_table_destroy(st);
}
