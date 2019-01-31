#include "test.h"
#include "core.c"
#include "gl-mock.c"

#define _hx(h) _dd(h.c, h.r)
#define _v2(v) _gg(v.x, v.y)

TEST(core_toggle_hex_at_point)
{
	Quad quad = {{0, 0}, {127, 63}};
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 250};
	f32 scale = 20.0;
	vec2 v = {537.8, 482.3};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);
	View *vw = malloc(sizeof *vw);

	gl_mock_initialize();
	grid_initialize(g, &quad);
	_hx(g->storage_quad.size);
	//=> 64, 64

	ui_initialize(ui, 0);
	ui_update_styles(ui, g);
	view_initialize(vw, viewport_size, translation, scale);

	Hex h = view_point_to_hex(vw, v);
	_hx(h);
	//=> 15, 29

	core_toggle_hex_at_point(ui, vw, g, v);

	_d(grid_has(g, h));
	//=> 1
	_d((i32) grid_get(g, h));
	//=> 1
	_dd(GL_MOCK.sub_texture_xoffset[1], GL_MOCK.sub_texture_yoffset[1]);
	//=> 7, 29
	_dd(GL_MOCK.texture_width[1], GL_MOCK.texture_height[1]);
	//=> 1, 1

	core_toggle_hex_at_point(ui, vw, g, v);
	_d(grid_has(g, h));
	//=> 0
	_d((i32) grid_get(g, h));
	//=> 0

	// Increment style index
	core_toggle_hex_at_point(ui, vw, g, v);
	_d((i32) grid_get(g, h));
	//=> 2

	// Ignores outside of grid quad
	v = (vec2) {342, 500};
	h = view_point_to_hex(vw, v);
	_hx(h);
	//=> -6, 30
	core_toggle_hex_at_point(ui, vw, g, v);

	grid_terminate(g);
	ui_terminate(ui);
	free(g);
	free(ui);
	free(vw);
}
