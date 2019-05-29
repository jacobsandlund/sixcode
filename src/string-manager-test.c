#include "test.h"
#include "string-manager.c"
#include "string-id.c"
#include "sid-hash-table.c"
#include "fnv.c"

Test(string_manager_init)
{
	string_manager_init(4);

	SidHashTable *sid_table = &gStringManager.string_id_table;

	_d(sid_table->size);
	//=> 4
	_d(sid_table->sizeof_value);
	//=> 8

	_u64(StringIdIntern("foo"));
	//=> 15902901984413996407

	_s(*((u8 **) sid_hash_table_get(sid_table, StringId("foo"))));
	//=> foo

	_s(StringIdString(StringId("foo")));
	//=> foo

	string_manager_destroy();
}
