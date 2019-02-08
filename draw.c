#include "draw.h"

void draw(Ui *ui, View *vw, Grid *g)
{
	glViewport(0, 0, vw->viewport_size.x, vw->viewport_size.y);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Quad viewport_quad;
	view_viewport_to_quad(vw, &viewport_quad);

	ui_draw_fill(ui, vw, g, &viewport_quad);
}
