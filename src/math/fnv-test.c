#include "math/fnv.c"
#include <string.h>
#include "test/test.h"

u64 test_hash_literal(const char *s)
{
    return fnv_hash((const u8 *) s, strlen(s));
}

Test(fnv_hash)
{
    u8 s[] = "foo";
    _u64(fnv_hash(s, strlen((char *) s)));
    //=> 15902901984413996407

    _u64(test_hash_literal("foo"));
    //=> 15902901984413996407
    _u64(test_hash_literal("foobar"));
    //=> 9625390261332436968
    _u64(test_hash_literal("foobas"));
    //=> 9625391360844065179
    _u64(test_hash_literal("goobas"));
    //=> 25052863860475014
    _u64(test_hash_literal(""));
    //=> 14695981039346656037
    _u64(test_hash_literal("The quick brown fox jumps over the lazy dog"));
    //=> 17580284887202820368
}

Test(fnv_fold)
{
    _u64(fnv_fold(test_hash_literal("foo"), 6, 63));
    //=> 34
    _u64(fnv_fold(test_hash_literal("foo"), 7, 127));
    //=> 93
    _u64(fnv_fold(test_hash_literal("foobar"), 16, 65535));
    //=> 37073
}
