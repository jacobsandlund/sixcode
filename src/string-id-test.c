#include "test.h"
#include <string.h>
#include "string-id.c"
#include "sid-hash-table.c"
#include "fnv.c"

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
	SidHashTable *ht = malloc(sizeof *ht);

	sid_hash_table_init(ht, 4, sizeof(u8 *));

	_d(ht->count);
	//=> 0

	u8 s1[] = "foo";
	_u64(string_id_intern(ht, (const u8 *) s1, strlen((char *) s1)));
	//=> 15902901984413996407
	_d(ht->count);
	//=> 1

	u8 s2[] = "The quick brown fox jumps over the lazy dog";
	_u64(string_id_intern(ht, (const u8 *) s2, strlen((char *) s2)));
	//=> 17580284887202820368
	_d(ht->count);
	//=> 2

	// Literal

	// And with the same string
	_u64(string_id_intern_literal(ht, "foo"));
	//=> 15902901984413996407
	_d(ht->count);  // Same count
	//=> 2

	_u64(string_id_intern_literal(ht, "foobar"));
	//=> 9625390261332436968
	_u64(string_id_intern_literal(ht, "foobas"));
	//=> 9625391360844065179

	_d(ht->count);
	//=> 4

	sid_hash_table_destroy(ht);

	free(ht);
}

Test(string_id_string)
{
	SidHashTable *ht = malloc(sizeof *ht);

	sid_hash_table_init(ht, 4, sizeof(u8 *));
	
	sid id = string_id_intern_literal(ht, "foobar");
	_u64(id);
	//=> 9625390261332436968

	_s(string_id_string(ht, id));
	//=> foobar

	_s(string_id_string(ht, string_id_literal("this won't be found")));	
	//=> !!! Not found !!!

	sid_hash_table_destroy(ht);

	free(ht);
}
