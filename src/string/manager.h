#ifndef StringManager_h
#define StringManager_h

#include "spacetime.h"
#include "string/id.h"
#include "string/id-table.h"

typedef struct {
	i32 string_table_size;
} StringManagerConfiguration;

typedef struct {
	StringIdTable string_table;
} StringManager;

extern StringManager gStringManager;

#define StringId(s) string_id_literal(s)
#define StringIdIntern(s) string_id_intern_literal(&gStringManager.string_table, s)
#define StringIdString(id) string_id_string(&gStringManager.string_table, id)

void string_manager_init(StringManagerConfiguration *config);
void string_manager_destroy(void);

#endif // StringManager_h
