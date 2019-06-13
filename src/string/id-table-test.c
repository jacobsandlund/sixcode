#include "string/id-table.c"
#include "math/fnv.c"
#include "test/test.h"

u64 test_hash_literal(const char *s)
{
    return fnv_hash((const u8 *) s, strlen(s));
}

char *test_table_insert_and_get_literal(StringIdTable *st, const char *s)
{
    sid key = test_hash_literal(s);
    string_id_table_insert(st, key, &s);
    return *((char **) string_id_table_get(st, key));
}

Test(string_id_table_init)
{
    StringIdTable st;

    string_id_table_init(&st, 200, sizeof(void *));

    _d(st.size);
    //=> 256
    _d(st.count);
    //=> 0
    _d(st.sizeof_value);
    //=> 8
    _d(st.mask);
    //=> 255
    _d(st.shift_bits);
    //=> 8

    _u64(st.keys[0]);
    //=> 0
    _u64(st.keys[255]);
    //=> 0

    string_id_table_destroy(&st);
}

Test(string_id_table_insert_and_get)
{
    StringIdTable *st = tmalloc(sizeof *st);

    string_id_table_init(st, 8, sizeof(char *));

    _d(st->size);
    //=> 8
    _d(st->count);
    //=> 0
    _d(st->sizeof_value);
    //=> 8

    // Get before any set

    char *s = "foo";
    _d(string_id_table_get(st, test_hash_literal(s)));
    //=> 0

    // Basic insert and get

    string_id_table_insert(st, test_hash_literal(s), &s);
    _d(st->count);
    //=> 1

    _s(*((char **) string_id_table_get(st, test_hash_literal(s))));
    //=> foo

    // Get without prior insert

    _d(string_id_table_get(st, test_hash_literal("foobar")));
    //=> 0

    // Insert up to full

    _s(test_table_insert_and_get_literal(st, "foobar"));
    //=> foobar
    _s(test_table_insert_and_get_literal(st, "foobas"));
    //=> foobas
    _s(test_table_insert_and_get_literal(st, "goobas"));
    //=> goobas
    _s(test_table_insert_and_get_literal(st, "42"));
    //=> 42
    _s(test_table_insert_and_get_literal(st, "yo yo"));
    //=> yo yo
    _s(test_table_insert_and_get_literal(st, "the chicken crossed the road"));
    //=> the chicken crossed the road
    _s(test_table_insert_and_get_literal(st, "chocotaco"));
    //=> chocotaco

    _d(st->count);
    //=> 8

    string_id_table_destroy(st);
}

Test(string_id_table_delete)
{
    StringIdTable *st = tmalloc(sizeof *st);

    string_id_table_init(st, 4, sizeof(char *));

    _s(test_table_insert_and_get_literal(st, "foobar"));
    //=> foobar
    _s(test_table_insert_and_get_literal(st, "foobas"));
    //=> foobas
    _s(test_table_insert_and_get_literal(st, "goobas"));
    //=> goobas
    _s(test_table_insert_and_get_literal(st, "go go go"));
    //=> go go go

    _d(st->count);
    //=> 4

    string_id_table_delete(st, test_hash_literal("foobar"));
    string_id_table_delete(st, test_hash_literal("goobas"));

    _d(string_id_table_get(st, test_hash_literal("foobar")));
    //=> 0
    _d(st->count);
    //=> 2

    _s(test_table_insert_and_get_literal(st, "yeah yeah yeah"));
    //=> yeah yeah yeah

    _d(st->count);
    //=> 3

    string_id_table_destroy(st);
}
