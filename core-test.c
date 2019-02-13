#include "test.h"
#include "core.c"
#include "glmock.c"

TEST(core_grid_expand_for_hex)
{
	Hex h1 = {5, 27};
	Hex h2 = {48, 62};
	Quad grid_quad = {{2, 1}, {125, 62}};
	Grid *g = malloc(sizeof *g);
	UiAll *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &grid_quad);
	ui_all_initialize(ui, 0);

	_qd(g->quad);
	//=> (2, 1), (125, 62)

	grid_set(g, h1, 1);
	grid_set(g, h2, 2);

	Hex h3 = {-270, 130};
	_d(core_grid_expand_for_hex(ui, g, h3));
	//=> 1

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->grid.textures.grid_styles];
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 256, 192
	_d(grid_get(g, h1));
	//=> 1
	_d(grid_get(g, h2));
	//=> 2
	_qd(g->quad);
	//=> (-382, 1), (125, 190)

	grid_set(g, h3, 3);

	Hex h4 = {300, -UI_GRID_MAX_TEXTURE_SIZE};
	_d(core_grid_expand_for_hex(ui, g, h4));
	//=> 0

	_d(grid_get(g, h3));
	//=> 3
	_qd(g->quad);
	//=> (-382, 1), (125, 190)

	grid_terminate(g);
	ui_all_terminate(ui);
	free(g);
	free(ui);
}

TEST(core_toggle_hex_at_point)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {100, 250},
		.scale = 20.0,
	};
	Quad grid_quad = {{2, 1}, {125, 62}};
	vec2 v = {537.8, 482.3};

	Grid *g = malloc(sizeof *g);
	UiAll *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &grid_quad);
	_hx(g->storage_quad.size);
	//=> 64, 64

	ui_all_initialize(ui, 0);
	ui_grid_update_styles(&ui->grid, g);

	Hex h = space_hex_round(space_world_to_hex(
			space_screen_to_world(&vw, v)));
	_hx(h);
	//=> 15, 29

	core_toggle_hex_at_point(ui, &vw, g, v);

	_d(grid_get(g, h));
	//=> 1

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->grid.textures.grid_styles];
	_dd(grid_styles_texture->xoffset, grid_styles_texture->yoffset);
	//=> 7, 29
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 1, 1

	core_toggle_hex_at_point(ui, &vw, g, v);
	_d(grid_get(g, h));
	//=> 0

	// Increment style index
	core_toggle_hex_at_point(ui, &vw, g, v);
	_d(grid_get(g, h));
	//=> 2

	// Ignores outside of grid quad
	v = (vec2) {342, 500};
	h = space_hex_round(space_world_to_hex(
			space_screen_to_world(&vw, v)));
	_hx(h);
	//=> -6, 30
	core_toggle_hex_at_point(ui, &vw, g, v);

	grid_terminate(g);
	ui_all_terminate(ui);
	free(g);
	free(ui);
}
