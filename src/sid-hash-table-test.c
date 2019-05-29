#include "test.h"
#include "sid-hash-table.c"
#include "fnv.c"

u64 test_hash_literal(const char *s)
{
	return fnv_hash((const u8 *) s, strlen(s));
}

char *test_table_insert_and_get_literal(SidHashTable *ht, const char *s)
{
	sid key = test_hash_literal(s);
	sid_hash_table_insert(ht, key, &s);
	return *((char **) sid_hash_table_get(ht, key));
}

Test(sid_hash_table_init)
{
	SidHashTable *ht = malloc(sizeof *ht);

	sid_hash_table_init(ht, 200, sizeof(void *));

	_d(ht->size);
	//=> 256
	_d(ht->count);
	//=> 0
	_d(ht->sizeof_value);
	//=> 8
	_d(ht->mask);
	//=> 255
	_d(ht->shift_bits);
	//=> 8

	_u64(ht->keys[0]);
	//=> 0
	_u64(ht->keys[255]);
	//=> 0

	sid_hash_table_destroy(ht);

	free(ht);
}

Test(sid_hash_table_insert_and_get)
{
	SidHashTable *ht = malloc(sizeof *ht);

	sid_hash_table_init(ht, 8, sizeof(char *));

	_d(ht->size);
	//=> 8
	_d(ht->count);
	//=> 0
	_d(ht->sizeof_value);
	//=> 8

	// Get before any set

	char *s = "foo";
	_d(sid_hash_table_get(ht, test_hash_literal(s)));
	//=> 0

	// Basic insert and get

	sid_hash_table_insert(ht, test_hash_literal(s), &s);
	_d(ht->count);
	//=> 1

	_s(*((char **) sid_hash_table_get(ht, test_hash_literal(s))));
	//=> foo

	// Get without prior insert

	_d(sid_hash_table_get(ht, test_hash_literal("foobar")));
	//=> 0

	// Insert up to full

	_s(test_table_insert_and_get_literal(ht, "foobar"));
	//=> foobar
	_s(test_table_insert_and_get_literal(ht, "foobas"));
	//=> foobas
	_s(test_table_insert_and_get_literal(ht, "goobas"));
	//=> goobas
	_s(test_table_insert_and_get_literal(ht, "42"));
	//=> 42
	_s(test_table_insert_and_get_literal(ht, "yo yo"));
	//=> yo yo
	_s(test_table_insert_and_get_literal(ht, "the chicken crossed the road"));
	//=> the chicken crossed the road
	_s(test_table_insert_and_get_literal(ht, "chocotaco"));
	//=> chocotaco

	_d(ht->count);
	//=> 8

	sid_hash_table_destroy(ht);

	free(ht);
}
