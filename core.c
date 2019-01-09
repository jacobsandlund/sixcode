#include "core.h"

typedef struct {
	i32 dummy;
} CoreDummyData;

static CoreDummyData CORE_DUMMY_DATA = {.dummy = 1};

void core_toggle_hex_at_point(Layout *l, Grid *g, Point p)
{
	Hex h = layout_point_to_hex(l, p);
	void *datum = grid_get(g, h);

	if (datum == NULL) {
		grid_set(g, h, &CORE_DUMMY_DATA);
	} else {
		grid_remove(g, h);
	}
}
