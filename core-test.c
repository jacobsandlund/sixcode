#include "test.h"
#include "core.c"
#include "glmock.c"

TEST(core_grid_expand_for_hex)
{
	Hex h1 = {5, 27};
	Hex h2 = {48, 62};
	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	core_grid_initialize(g);
	ui_initialize(ui, 0);

	_hx(g->quad.min);
	//=> 2, 1
	_hx(g->quad.max);
	//=> 125, 62

	grid_set(g, h1, 1);
	grid_set(g, h2, 2);

	Hex h3 = {-270, 130};
	_d(core_grid_expand_for_hex(ui, g, h3));
	//=> 1

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->textures.grid_styles];
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 256, 192
	_d(grid_get(g, h1));
	//=> 1
	_d(grid_get(g, h2));
	//=> 2
	_hx(g->quad.min);
	//=> -382, 1
	_hx(g->quad.max);
	//=> 125, 190

	grid_set(g, h3, 3);

	Hex h4 = {300, -UI_MAX_TEXTURE_SIZE};
	_d(core_grid_expand_for_hex(ui, g, h4));
	//=> 0

	_d(grid_get(g, h3));
	//=> 3
	_hx(g->quad.min);
	//=> -382, 1

	grid_terminate(g);
	ui_terminate(ui);
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
	vec2 v = {537.8, 482.3};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	core_grid_initialize(g);
	_hx(g->storage_quad.size);
	//=> 64, 64

	ui_initialize(ui, 0);
	ui_update_styles(ui, g);

	Hex h = hex_round(view_point_to_vec_hex(&vw, v));
	_hx(h);
	//=> 15, 29

	core_toggle_hex_at_point(ui, &vw, g, v);

	_d(grid_get(g, h));
	//=> 1

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->textures.grid_styles];
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
	h = hex_round(view_point_to_vec_hex(&vw, v));
	_hx(h);
	//=> -6, 30
	core_toggle_hex_at_point(ui, &vw, g, v);

	grid_terminate(g);
	ui_terminate(ui);
	free(g);
	free(ui);
}
