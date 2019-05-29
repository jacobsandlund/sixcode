#ifndef StringId_h
#define StringId_h

#include "spacetime.h"
#include "sid-hash-table.h"
#include "fnv.h"

void string_id_table_init(i32 size);
void string_id_table_destroy(void);
i32 string_id_table_count(void);

static inline sid string_id(const u8 *s, i64 length)
{
	return (sid) fnv_hash(s, length);
}

sid string_id_literal(const char *s);
sid string_id_intern(SidHashTable *ht, const u8 *s, i64 length);
sid string_id_intern_literal(SidHashTable *ht, const char *s);
u8 *string_id_string(SidHashTable *ht, sid id);

#endif // StringId_h
