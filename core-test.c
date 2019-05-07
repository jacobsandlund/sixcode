#include "core.c"
#include "test.h"
#include "glmock.c"

TEST(core_tick)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 100};
	float scale = 8.0;

	View *vw = malloc(sizeof *vw);
	Grid *g = malloc(sizeof *g);
	UiAll *ui = malloc(sizeof *ui);

	glmock_initialize();
	view_initialize(vw, viewport_size, translation, scale);
	grid_initialize(g);
	ui_all_initialize(ui);
	texture_update(&ui->fill.grid_styles_texture, g);

	Quad viewport_quad;
	view_viewport_to_quad(vw, &viewport_quad);
	_qd(viewport_quad);
	//=> (63, 75), (136, 125)

	core_tick(ui, vw, g);

	_d(GLmock.viewport_width);
	//=> 1000
	_d(GLmock.viewport_height);
	//=> 600

	_d(GLmock.draw_elements_count);
	//=> 53760

	ui_all_terminate(ui);
	grid_terminate(g);

	free(vw);
	free(g);
	free(ui);
}

TEST(core_toggle_hex_at_point)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 250};
	float scale = 20.0;
	vec2 v = {537.8, 482.3};

	View *vw = malloc(sizeof *vw);
	Grid *g = malloc(sizeof *g);
	UiAll *ui = malloc(sizeof *ui);

	glmock_initialize();
	view_initialize(vw, viewport_size, translation, scale);
	grid_initialize(g);
	_i2(g->size_quad.size);
	//=> 4096, 4096

	ui_all_initialize(ui);
	texture_update(&ui->fill.grid_styles_texture, g);

	ivec2 h = view_world_round(vw, view_screen_to_world(vw, v));
	_i2(h);
	//=> 101, 256

	core_toggle_hex_at_point(ui, vw, g, v);

	_d(grid_get(g, h));
	//=> 1

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->fill.grid_styles_texture.texture];
	_dd(grid_styles_texture->xoffset, grid_styles_texture->yoffset);
	//=> 2149, 2304
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

	grid_terminate(g);
	ui_all_terminate(ui);
	free(vw);
	free(g);
	free(ui);
}
