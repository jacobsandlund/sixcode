#include <assert.h>
#include <string.h>
#include "sid-hash-table.h"
#include "fnv.h"
#include "bit.h"

void sid_hash_table_init(SidHashTable *ht, i32 size, i64 sizeof_value)
{
	ht->size = size = (i32) bit_next_power_of_two((u64) size);
	ht->count = 0;
	ht->sizeof_value = (i32) sizeof_value;
	ht->mask = (u64) (size - 1);
	ht->shift_bits = bit_highest_bit(ht->mask);
	ht->keys = calloc(size, sizeof *ht->keys);
	ht->values = malloc(size * (i32) sizeof_value);
}

void sid_hash_table_destroy(SidHashTable *ht)
{
	free(ht->keys);
	free(ht->values);
}

void sid_hash_table_insert(SidHashTable *ht, sid key, const void *value)
{
	assert(sid_hash_table_get(ht, key) == NULL);
	assert(ht->count < ht->size);
	u64 mask = ht->mask;
	u64 k = fnv_fold((u64) key, ht->shift_bits, mask);
	sid *keys = ht->keys;
	while (keys[k]) {
		k = (k + 1) & mask;
	}

	keys[k] = key;
	memcpy(&ht->values[k * ht->sizeof_value], value, ht->sizeof_value);
	ht->count++;
}

void *sid_hash_table_get(SidHashTable *ht, sid search_key)
{
	u64 mask = ht->mask;
	u64 k = fnv_fold((u64) search_key, ht->shift_bits, mask);
	sid *keys = ht->keys;
	sid key;

	while ((key = keys[k])) {
		if (key == search_key) {
			return &ht->values[k * ht->sizeof_value];
		}

		k = (k + 1) & mask;
	}

	return NULL;
}
