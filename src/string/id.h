#ifndef StringId_h
#define StringId_h

#include "spacetime.h"
#include "string/id-table.h"
#include "math/fnv.h"

static inline sid string_id(const u8 *s, i64 length)
{
    return (sid) fnv_hash(s, length);
}

sid string_id_literal(const char *s);
sid string_id_intern(StringIdTable *st, const u8 *s, i64 length);
sid string_id_intern_literal(StringIdTable *st, const char *s);
u8 *string_id_string(StringIdTable *st, sid id);

#endif // StringId_h
