#include "test.h"
#include "glmock.c"
#include "grid.c"
#include "hex.c"
#include "mesh.c"
#include "shader.c"
#include "space.c"
#include "quad.c"
#include "ui-all.c"
#include "ui-grid.c"
#include "ui-fill.c"
#include "ui-points.c"
#include "ui-stroke.c"
#include "view.c"

TEST(ui_all)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {100, 100},
		.scale = 8.0,
	};
	Quad quad = {{-126, 1}, {253, 126}};

	Grid *g = malloc(sizeof *g);
	UiAll *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &quad);
	ui_all_initialize(ui, 0);
	ui_grid_update_styles(&ui->grid, g);

	Quad viewport_quad;
	view_viewport_to_quad(&vw, &viewport_quad);
	_qd(viewport_quad);
	//=> (-116, -34), (174, 67)

	// Zoom with stroke and fill

	ui_all_draw(ui, &vw, g);

	_d(GLmock.viewport_width);
	//=> 1000
	_d(GLmock.viewport_height);
	//=> 600

	_d(GLmock.draw_elements_count);
	//=> 294912
	_d(GLmock.draw_arrays_count);
	//=> 0

	// Zoom with fill and no stroke

	vw.scale = 6.0;
	GLmock.draw_elements_count = 0;
	GLmock.draw_arrays_count = 0;

	ui_all_draw(ui, &vw, g);

	_d(GLmock.draw_elements_count);
	//=> 294912
	_d(GLmock.draw_arrays_count);
	//=> 0

	// Zoom with only points

	vw.scale = 1.0;
	GLmock.draw_elements_count = 0;
	GLmock.draw_arrays_count = 0;

	ui_all_draw(ui, &vw, g);

	_d(GLmock.draw_elements_count);
	//=> 0
	_d(GLmock.draw_arrays_count);
	//=> 65536

	ui_all_terminate(ui);
	grid_terminate(g);

	free(g);
	free(ui);
}

TEST(ui_all_initialize_fail)
{
	Quad quad = {{-126, 1}, {253, 126}};

	Grid *g = malloc(sizeof *g);
	UiAll *ui = malloc(sizeof *ui);

	glmock_initialize();
	GLmock.shaders[3].compiled = -1;
	grid_initialize(g, &quad);

	_d(ui_all_initialize(ui, 0));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error compiling shader. Nothing in info log.
	//=>

	grid_terminate(g);
	free(g);
	free(ui);
}
