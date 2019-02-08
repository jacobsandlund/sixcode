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

#define _hx(h) _dd(h.c, h.r)

TEST(draw)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {100, 100},
		.scale = 10.0,
	};
	Quad quad = {{-128, 0}, {255, 127}};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &quad);
	ui_initialize(ui, 0);
	ui_update_styles(ui, g);

	Quad viewport_quad;
	view_viewport_to_quad(&vw, &viewport_quad);
	_hx(viewport_quad.min);
	//=> -93, -27
	_hx(viewport_quad.max);
	//=> 139, 54

	draw(ui, &vw, g);

	_d(GLmock.viewport_width);
	//=> 1000
	_d(GLmock.viewport_height);
	//=> 600

	ui_terminate(ui);
	grid_terminate(g);

	free(g);
	free(ui);
}
