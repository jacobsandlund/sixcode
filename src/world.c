#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "world.h"
#include "layout.h"

#define WORLD_HEX_FILL_MIN 1
#define WORLD_HEX_FILL_MAX 15

void world_initialize(World *w)
{
	grid_initialize(&w->grid);

	Viewport *vp = &w->viewport;
	vp->camera = (float3) {0.0, 0.0, 16.0};
	vp->size = (float2) {640, 480};
	layout_type(&vp->layout, LayoutTypeHex);
}

void world_terminate(World *w)
{
	grid_terminate(&w->grid);
}

void world_load(World *w)
{
	srand((unsigned int) time(NULL));
	i64 count = 6000000;
	double size = 4096.0 - 2.0;
	i64 style = 0;

	for (i64 i = 0; i < count; i++) {
		double rand1 = (double) rand() / (double) RAND_MAX;
		double rand2 = (double) rand() / (double) RAND_MAX;
		int2 h = {
			floor(rand1 * size) - 4096/2 + 1,
			floor(rand2 * size) - 4096/2 + 1,
		};

		style++;
		if (style == 16) {
			style = 1;
		}

		grid_set(&w->grid, h, style);
	}
}

void world_update(World *w, EventQueue *eq)
{
	u64 time = event_queue_clock_time();
	Event event;
	i64 num_behind = eq->next_write_event_id - eq->next_read_event_id;
	if (num_behind >= eq->length) {
		SPACETIME_ERROR(
	}

	while (num_behind > 0 && num_behind < eq->length) {
		event_queue_read(eq, &event);
	}
	(void) w;
	(void) eq;
}

/*
void world_toggle_hex_at_point(World *w, vec2 v)
{
	static u8 style = WORLD_HEX_FILL_MIN;

	ivec2 h = camera_world_vector_round(c, camera_screen_to_world_vector(c, v));

	if (!quad_contains(&g->quad, h)) {
		return;
	}

	if (grid_get(g, h)) {
		grid_clear(g, h);
	} else {
		grid_set(g, h, style);

		++style;
		if (style > WORLD_HEX_FILL_MAX) {
			style = WORLD_HEX_FILL_MIN;
		}
	}

	Quad quad = {h, h};
	texture_update_in_quad(&ui->grid_styles_texture, g, &quad);
}
*/
