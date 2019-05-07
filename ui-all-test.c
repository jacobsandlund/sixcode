#include "ui-all.c"
#include "test.h"
#include "glmock.c"
#include "grid.c"
#include "mesh.c"
#include "shader.c"
#include "quad.c"
#include "ui-grid.c"
#include "ui-fill.c"
#include "view.c"

TEST(ui_all)
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
	ui_all_initialize(ui, 0);
	ui_grid_update_styles(&ui->grid, g);

	Quad viewport_quad;
	view_viewport_to_quad(vw, &viewport_quad);
	_qd(viewport_quad);
	//=> (63, 75), (136, 125)

	// Zoom 1

	ui_all_draw(ui, vw, g);

	_d(GLmock.viewport_width);
	//=> 1000
	_d(GLmock.viewport_height);
	//=> 600

	_d(GLmock.draw_elements_count);
	//=> 53760

	// Zoom 2

	scale = 6.0;
	view_initialize(vw, viewport_size, translation, scale);
	GLmock.draw_elements_count = 0;
	GLmock.draw_arrays_count = 0;

	ui_all_draw(ui, vw, g);

	_d(GLmock.draw_elements_count);
	//=> 89856

	// Zoom 3

	scale = 1.0;
	view_initialize(vw, viewport_size, translation, scale);
	GLmock.draw_elements_count = 0;
	GLmock.draw_arrays_count = 0;

	ui_all_draw(ui, vw, g);

	_d(GLmock.draw_elements_count);
	//=> 2859264

	ui_all_terminate(ui);
	grid_terminate(g);

	free(vw);
	free(g);
	free(ui);
}

TEST(ui_all_initialize_fail)
{
	Grid *g = malloc(sizeof *g);
	UiAll *ui = malloc(sizeof *ui);

	glmock_initialize();
	GLmock.shaders[1].force_compile_error = true;
	grid_initialize(g);

	_d(ui_all_initialize(ui, 0));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error compiling shader. Nothing in info log.
	//=>

	grid_terminate(g);
	free(g);
	free(ui);
}
