#ifndef StringIdTable_h
#define StringIdTable_h

#include "spacetime.h"

#define StringIdTableTombstone 1

typedef struct {
	i32 size;
	i32 count;
	i32 sizeof_value;
	u64 mask;
	u64 shift_bits;
	sid *keys;
	void *values;
} StringIdTable;

void string_id_table_init(StringIdTable *st, i32 size, i64 sizeof_value);
void string_id_table_destroy(StringIdTable *st);
void string_id_table_insert(StringIdTable *st, sid key, const void *value);
void *string_id_table_get(StringIdTable *st, sid search_key);
void string_id_table_delete(StringIdTable *st, sid key);

#endif // StringIdTable_h
