#include "string/manager.h"

StringManager gStringManager;

void string_manager_init(StringManagerConfig *config)
{
    string_id_table_init(&gStringManager.string_table, config->string_table_size, sizeof(u8 *));
}

void string_manager_destroy(void)
{
    string_id_table_destroy(&gStringManager.string_table);
}
