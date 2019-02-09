#include "test.h"
#include "draw.c"
#include "glmock.c"
#include "grid.c"
#include "hex.c"
#include "matrix.c"
#include "mesh.c"
#include "shader.c"
#include "quad.c"
#include "ui.c"
#include "view.c"

TEST(draw)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {100, 100},
		.scale = 20.0,
	};
	Quad quad = {{-126, 1}, {253, 126}};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &quad);
	ui_initialize(ui, 0);
	ui_update_styles(ui, g);

	Quad viewport_quad;
	view_viewport_to_quad(&vw, &viewport_quad);
	_hx(viewport_quad.min);
	//=> -47, -14
	_hx(viewport_quad.max);
	//=> 70, 27

	// Normal zoom with stroke and fill

	draw(ui, &vw, g);

	_d(GLmock.viewport_width);
	//=> 1000
	_d(GLmock.viewport_height);
	//=> 600

	_d(GLmock.draw_elements_count);
	//=> 49152
	_d(GLmock.draw_arrays_count);
	//=> 24576

	// Zoomed out far with no stroke

	vw.scale = 5.0;
	GLmock.draw_elements_count = 0;
	GLmock.draw_arrays_count = 0;

	draw(ui, &vw, g);

	_d(GLmock.draw_elements_count);
	//=> 294912
	_d(GLmock.draw_arrays_count);
	//=> 0

	ui_terminate(ui);
	grid_terminate(g);

	free(g);
	free(ui);
}
