#include <stdlib.h>
#include <string.h>
#include "string-id.h"
#include "sid-hash-table.h"
#include "fnv.h"

sid string_id_literal(const char *s)
{
	return string_id((const u8 *) s, strlen(s));
}

sid string_id_intern(SidHashTable *ht, const u8 *s, i64 length)
{
	sid id = string_id(s, length);
	if (id == 0) {
		Log("Found string with sid == 0: %s");
		abort();
	}

	u8 **other_string = (u8 **) sid_hash_table_get(ht, id);
	if (other_string) {
		if (strcmp((const char *) s, (char *) *other_string) != 0) {
			Log("Found string id (sid) collision with strings: '%s', '%s'",
					s, *other_string);
			abort();
		}

		// Already inserted. Do nothing
	} else {
		sid_hash_table_insert(ht, id, &s);
	}

	return id;
}

sid string_id_intern_literal(SidHashTable *ht, const char *s)
{
	return string_id_intern(ht, (const u8 *) s, strlen(s));
}

u8 *string_id_string(SidHashTable *ht, sid id)
{
	u8 **s = sid_hash_table_get(ht, id);
	if (s) {
		return *s;
	}

	return (u8 *) "!!! Not found !!!";
}
