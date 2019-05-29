#include "string-manager.h"

StringManager gStringManager;

void string_manager_start_up(i32 string_id_table_size)
{
	sid_hash_table_init(&gStringManager.string_id_table, string_id_table_size, sizeof(u8 *));
}

void string_manager_shut_down(void)
{
	sid_hash_table_destroy(&gStringManager.string_id_table);
}
