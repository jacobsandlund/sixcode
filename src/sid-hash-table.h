#ifndef SidHashTable_h
#define SidHashTable_h

#include "spacetime.h"

typedef struct {
	i32 size;
	i32 count;
	i32 sizeof_value;
	u64 mask;
	u64 shift_bits;
	sid *keys;
	void *values;
} SidHashTable;

void sid_hash_table_init(SidHashTable *ht, i32 size, i64 sizeof_value);
void sid_hash_table_destroy(SidHashTable *ht);
void sid_hash_table_insert(SidHashTable *ht, sid key, const void *value);
void *sid_hash_table_get(SidHashTable *ht, sid search_key);

#endif // SidHashTable_h
