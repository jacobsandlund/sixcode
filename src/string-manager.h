#ifndef StringManager_h
#define StringManager_h

#include "spacetime.h"
#include "sid-hash-table.h"
#include "string-id.h"

typedef struct {
	SidHashTable string_id_table;
} StringManager;

extern StringManager gStringManager;

#define StringId(s) string_id_literal(s)
#define StringIdIntern(s) string_id_intern_literal(&gStringManager.string_id_table, s)
#define StringIdString(id) string_id_string(&gStringManager.string_id_table, id)

void string_manager_init(i32 string_id_table_size);
void string_manager_destroy(void);

#endif // StringManager_h
