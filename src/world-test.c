#include "world.c"
#include "test.h"
#include "glmock.c"

TEST(world_tick)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 100};
	float scale = 8.0;

	Camera *c = malloc(sizeof *c);
	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	camera_initialize(c, viewport_size, translation, scale);
	grid_initialize(g);
	ui_initialize(ui);
	texture_update(&ui->grid_styles_texture, g);

	Quad viewport_quad;
	camera_viewport_to_quad(c, &viewport_quad);
	_qd(viewport_quad);
	//=> (63, 75), (136, 125)

	world_tick(ui, c, g);

	_d(GLmock.viewport_width);
	//=> 1000
	_d(GLmock.viewport_height);
	//=> 600

	_d(GLmock.draw_elements_count);
	//=> 53760

	ui_terminate(ui);
	grid_terminate(g);

	free(c);
	free(g);
	free(ui);
}

TEST(world_toggle_hex_at_point)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 250};
	float scale = 20.0;
	vec2 v = {537.8, 482.3};

	Camera *c = malloc(sizeof *c);
	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	camera_initialize(c, viewport_size, translation, scale);
	grid_initialize(g);
	_i2(g->size_quad.size);
	//=> 4096, 4096

	ui_initialize(ui);
	texture_update(&ui->grid_styles_texture, g);

	ivec2 h = camera_world_vector_round(c, camera_screen_to_world_vector(c, v));
	_i2(h);
	//=> 101, 256

	world_toggle_hex_at_point(ui, c, g, v);

	_d(grid_get(g, h));
	//=> 1

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->grid_styles_texture.texture];
	_dd(grid_styles_texture->xoffset, grid_styles_texture->yoffset);
	//=> 2149, 2304
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 1, 1

	// Toggles to zero
	world_toggle_hex_at_point(ui, c, g, v);
	_d(grid_get(g, h));
	//=> 0

	// Increment style index
	world_toggle_hex_at_point(ui, c, g, v);
	_d(grid_get(g, h));
	//=> 2

	grid_terminate(g);
	ui_terminate(ui);
	free(c);
	free(g);
	free(ui);
}
