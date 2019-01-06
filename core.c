#include "core.h"

typedef struct {
	i32 dummy;
} CoreDummyData;

static CoreDummyData CORE_DUMMY_DATA = {.dummy = 1};

void core_toggle_hex_at_point(Layout *l, Group *g, Point p)
{
	Hex h = layout_point_to_hex(l, p);
	void *datum = group_get(g, h);

	if (datum == NULL) {
		group_set(g, h, &CORE_DUMMY_DATA);
	} else {
		group_remove(g, h);
	}
}
