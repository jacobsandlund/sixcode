#include "string/id.h"
#include <stdlib.h>
#include <string.h>
#include "string/id-table.h"
#include "math/fnv.h"

sid string_id_literal(const char *s)
{
	return string_id((const u8 *) s, strlen(s));
}

sid string_id_intern(StringIdTable *st, const u8 *s, i64 length)
{
	sid id = string_id(s, length);
	if (id <= StringIdTableTombstone) {
		Log("Found string with sid == %" PRIu64 ": %s", id, s);
		abort();
	}

	u8 **other_string = (u8 **) string_id_table_get(st, id);
	if (other_string) {
		if (strcmp((const char *) s, (char *) *other_string) != 0) {
			Log("Found string id (sid) collision with strings: '%s', '%s'",
					s, *other_string);
			abort();
		}

		// Already inserted. Do nothing
	} else {
		string_id_table_insert(st, id, &s);
	}

	return id;
}

sid string_id_intern_literal(StringIdTable *st, const char *s)
{
	return string_id_intern(st, (const u8 *) s, strlen(s));
}

u8 *string_id_string(StringIdTable *st, sid id)
{
	u8 **s = string_id_table_get(st, id);
	if (s) {
		return *s;
	}

	return (u8 *) "!!! Not found !!!";
}
