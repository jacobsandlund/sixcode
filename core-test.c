#include "core.c"
#include "test.h"
#include "glmock.c"

TEST(core_tick)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 100};
	float scale = 8.0;
	Quad quad = {{-126, 1}, {253, 126}};

	View *vw = malloc(sizeof *vw);
	Grid *g = malloc(sizeof *g);
	UiAll *ui = malloc(sizeof *ui);

	glmock_initialize();
	view_initialize(vw, viewport_size, translation, scale);
	grid_initialize(g, &quad);
	ui_all_initialize(ui, 0);
	ui_grid_update_styles(&ui->grid, g);

	Quad viewport_quad;
	view_viewport_to_quad(vw, &viewport_quad);
	_qd(viewport_quad);
	//=> (-116, -34), (174, 67)

	core_tick(ui, vw, g);

	_d(GLmock.viewport_width);
	//=> 1000
	_d(GLmock.viewport_height);
	//=> 600

	_d(GLmock.draw_elements_count);
	//=> 131328

	ui_all_terminate(ui);
	grid_terminate(g);

	free(vw);
	free(g);
	free(ui);
}

TEST(core_grid_expand_for_hex)
{
	ivec2 h1 = {5, 27};
	ivec2 h2 = {48, 62};
	Quad grid_quad = {{2, 1}, {125, 62}};
	Grid *g = malloc(sizeof *g);

	glmock_initialize();
	grid_initialize(g, &grid_quad);

	_qd(g->quad);
	//=> (2, 1), (125, 62)

	grid_set(g, h1, 1);
	grid_set(g, h2, 2);

	ivec2 h3 = {-270, 130};
	_d(core_grid_expand_for_hex(g, h3));
	//=> 1

	_d(grid_get(g, h1));
	//=> 1
	_d(grid_get(g, h2));
	//=> 2
	_qd(g->quad);
	//=> (-382, 1), (125, 190)

	grid_set(g, h3, 3);

	ivec2 h4 = {300, -UI_GRID_MAX_TEXTURE_SIZE};
	_d(core_grid_expand_for_hex(g, h4));
	//=> 0

	_d(grid_get(g, h3));
	//=> 3
	_qd(g->quad);
	//=> (-382, 1), (125, 190)

	grid_terminate(g);
	free(g);
}

TEST(core_toggle_hex_at_point)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 250};
	float scale = 20.0;
	Quad grid_quad = {{2, 1}, {125, 62}};
	vec2 v = {537.8, 482.3};

	View *vw = malloc(sizeof *vw);
	Grid *g = malloc(sizeof *g);
	UiAll *ui = malloc(sizeof *ui);

	glmock_initialize();
	view_initialize(vw, viewport_size, translation, scale);
	grid_initialize(g, &grid_quad);
	_i2(g->storage_quad.size);
	//=> 64, 64

	ui_all_initialize(ui, 0);
	ui_grid_update_styles(&ui->grid, g);

	ivec2 h = hex_coords_round(view_world_to_hex(
			view_screen_to_world(vw, v)));
	_i2(h);
	//=> 15, 29

	core_toggle_hex_at_point(ui, vw, g, v);

	_d(grid_get(g, h));
	//=> 1

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->grid.textures.grid_styles];
	_dd(grid_styles_texture->xoffset, grid_styles_texture->yoffset);
	//=> 7, 29
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 1, 1

	// Toggles to zero
	core_toggle_hex_at_point(ui, vw, g, v);
	_d(grid_get(g, h));
	//=> 0

	// Increment style index
	core_toggle_hex_at_point(ui, vw, g, v);
	_d(grid_get(g, h));
	//=> 2

	// Expands when outside of grid quad

	v = (vec2) {0, 100};
	h = hex_coords_round(view_world_to_hex(
			view_screen_to_world(vw, v)));
	_i2(h);
	//=> -47, 3

	core_toggle_hex_at_point(ui, vw, g, v);
	_d(grid_get(g, h));
	//=> 3
	_dd(grid_styles_texture->xoffset, grid_styles_texture->yoffset);
	//=> 7, 29
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 128, 64

	grid_terminate(g);
	ui_all_terminate(ui);
	free(vw);
	free(g);
	free(ui);
}
