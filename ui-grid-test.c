#include "ui-grid.c"
#include "test.h"
#include "glmock.c"
#include "grid.c"
#include "hex-coords.c"
#include "quad.c"
#include "shader.c"
#include "view.c"

TEST(ui_grid)
{
	UiGrid *ui = malloc(sizeof *ui);

	glmock_initialize();

	_d(ui_grid_initialize(ui, 1024));
	//=> 1

	///////////////////////
	// load/create

	_d(ui->fragment_shader);
	//=> 1
	
	GLmockShader *fragment = &GLmock.shaders[ui->fragment_shader];
	_dd(fragment->created, fragment->compiled);
	//=> 1, 1

	/////////////////////
	// textures

	_d(ui->textures.grid_styles);
	//=> 1
	_d(ui->textures.fill_colors);
	//=> 2

	GLmockTexture *fill_colors_texture = &GLmock.textures[ui->textures.fill_colors];
	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->textures.grid_styles];

	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_WRAP_S) == GL_CLAMP_TO_EDGE);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_WRAP_T) == GL_CLAMP_TO_EDGE);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_MAG_FILTER) == GL_NEAREST);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_MIN_FILTER) == GL_NEAREST);
	//=> 1

	_d(glmock_get_tex_parameter(ui->textures.grid_styles, GL_TEXTURE_WRAP_S) == GL_CLAMP_TO_EDGE);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.grid_styles, GL_TEXTURE_WRAP_T) == GL_CLAMP_TO_EDGE);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.grid_styles, GL_TEXTURE_MAG_FILTER) == GL_NEAREST);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.grid_styles, GL_TEXTURE_MIN_FILTER) == GL_NEAREST);
	//=> 1

	_d(fill_colors_texture->created);
	//=> 1
	_dd(fill_colors_texture->width, fill_colors_texture->height);
	//=> 256, 1
	_d(fill_colors_texture->format == GL_RGB);
	//=> 1
	_d(fill_colors_texture->type == GL_UNSIGNED_BYTE);
	//=> 1
	_d(fill_colors_texture->data == UI_GRID_FILL_COLORS);
	//=> 1

	_d(grid_styles_texture->created);
	//=> 1

	//////////////////////
	// styles_buffer

	_d(ui->styles_buffer != 0);
	//=> 1
	_d(ui->styles_buffer_capacity);
	//=> 256
	_d(ui->styles_buffer_capacity_max);
	//=> 1024

	///////////////////////
	// view_matrix

	_gggg(ui->view_matrix.m[0][0], ui->view_matrix.m[1][0], ui->view_matrix.m[2][3], ui->view_matrix.m[3][3]);
	//=> 0, 0, 0, 0

	////////////////////////////
	// terminate

	ui_grid_terminate(ui);

	_d(fragment->deleted);
	//=> 1
	_d(grid_styles_texture->deleted);
	//=> 1

	free(ui);
}

TEST(ui_grid_initialize_fail)
{
	UiGrid *ui = malloc(sizeof *ui);

	glmock_initialize();
	GLmock.shaders[1].force_compile_error = true;

	_d(ui_grid_initialize(ui, 0));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error compiling shader. Nothing in info log.
	//=>

	free(ui);
}

TEST(ui_grid_size_quad_for_draw)
{
	SizeQuad out_sq;
	Quad grid_styles_quad = {{0, 0}, {127, 63}};
	Quad viewport_quad = {{-5, -5}, {59, 29}};

	// Intersect

	ui_grid_size_quad_for_draw(&out_sq, &grid_styles_quad, &viewport_quad);

	_sq(out_sq);
	//=> (0, 0), (30, 30)

	viewport_quad.min = (ivec2) {5, 5};

	// Even-align

	ui_grid_size_quad_for_draw(&out_sq, &grid_styles_quad, &viewport_quad);

	_sq(out_sq);
	//=> (2, 4), (28, 26)
}

TEST(ui_grid_update_view_matrix)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {100, 100},
		.scale = 10.0,
	};
	UiGrid *ui = malloc(sizeof *ui);

	glmock_initialize();
	ui_grid_initialize(ui, 0);

	vec2 offset = {93.0f, -13.0f};
	ui_grid_update_view_matrix(ui, &vw, offset);

	mat4 *m = &ui->view_matrix;
	_gggg(m->m[0][0], m->m[0][1], m->m[0][2], m->m[0][3]);
	//=> 0.001, 0, 0, 0
	_gggg(m->m[1][0], m->m[1][1], m->m[1][2], m->m[1][3]);
	//=> 0, 0.00166667, 0, 0
	_gggg(m->m[2][0], m->m[2][1], m->m[2][2], m->m[2][3]);
	//=> 0, 0, 0, 0
	_gggg(m->m[3][0], m->m[3][1], m->m[3][2], m->m[3][3]);
	//=> 0.073, 0.0116667, 0, 0.1

	ui_grid_terminate(ui);
	free(ui);
}

TEST(ui_grid_update_styles)
{
	Quad quad = {{2, 1}, {125, 126}};

	Grid *g = malloc(sizeof *g);
	UiGrid *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &quad);
	ui_grid_initialize(ui, 0);
	GLmock.bound_textures[0] = 0;

	ui_grid_update_styles(ui, g);

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->textures.grid_styles];

	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 64, 128
	_d(grid_styles_texture->format == GL_ALPHA);
	//=> 1
	_d(grid_styles_texture->type == GL_UNSIGNED_BYTE);
	//=> 1
	_d(grid_styles_texture->data == g->styles);
	//=> 1

	_d(GLmock.bound_textures[0] == ui->textures.grid_styles);
	//=> 1

	grid_terminate(g);
	ui_grid_terminate(ui);

	free(g);
	free(ui);
}

TEST(ui_grid_update_styles_in_quad)
{
	Quad quad;
	SizeQuad sq;
	Quad grid_quad = {{2, 1}, {125, 62}};

	Grid *g = malloc(sizeof *g);
	UiGrid *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &grid_quad);

	_d(ui_grid_initialize(ui, 2500));
	//=> 1
	_d(ui->styles_buffer_capacity);
	//=> 256

	ui_grid_update_styles(ui, g);

	// Under the current capacity
	quad = (Quad) {{10, 20}, {31, 59}};
	quad_to_storage_size_quad(&sq, &quad);
	_i2(ivec2_sub(sq.min, g->storage_quad.min));
	//=> 5, 20
	_i2(sq.size);
	//=> 11, 40
	
	ui_grid_update_styles_in_quad(ui, g, &quad);

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->textures.grid_styles];

	_dd(grid_styles_texture->xoffset, grid_styles_texture->yoffset);
	//=> 5, 20
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 11, 40
	_d(grid_styles_texture->format == GL_ALPHA);
	//=> 1
	_d(grid_styles_texture->type == GL_UNSIGNED_BYTE);
	//=> 1
	_d(grid_styles_texture->data == ui->styles_buffer);
	//=> 1
	_d(ui->styles_buffer_capacity);
	//=> 440

	// Under/equal the max capacity
	quad = (Quad) {{10, 10}, {109, 59}};
	quad_to_storage_size_quad(&sq, &quad);
	_i2(sq.size);
	//=> 50, 50
	_d(size_quad_capacity(&sq));
	//=> 2500

	ui_grid_update_styles_in_quad(ui, g, &quad);

	_d(ui->styles_buffer_capacity);
	//=> 2500
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 50, 50

	// Over the max capacity
	quad = (Quad) {{10, 10}, {109, 60}};
	quad_to_storage_size_quad(&sq, &quad);
	_i2(ivec2_sub(sq.min, g->storage_quad.min));
	//=> 5, 10
	_i2(sq.size);
	//=> 50, 51
	_i2(g->storage_quad.size);
	//=> 64, 64

	ui_grid_update_styles_in_quad(ui, g, &quad);

	_d(ui->styles_buffer_capacity);
	//=> 2500
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 64, 64

	grid_terminate(g);
	ui_grid_terminate(ui);

	free(g);
	free(ui);
}
