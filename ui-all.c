#include "ui-all.h"

#define UI_ALL_NUM_COMPONENTS 1

bool ui_all_initialize(UiAll *ui)
{
	bool initialized[UI_ALL_NUM_COMPONENTS] = {
		[0] = ui_fill_initialize(&ui->fill),
	};

	int count = 0;
	for (int i = 0; i < UI_ALL_NUM_COMPONENTS; ++i) {
		if (initialized[i]) {
			++count;
		}
	}

	if (count < UI_ALL_NUM_COMPONENTS) {

		// Indices need to match above
		if (initialized[0]) ui_fill_terminate(&ui->fill);

		return false;
	}

	return true;
}

void ui_all_terminate(UiAll *ui)
{
	ui_fill_terminate(&ui->fill);
}

void ui_all_draw(UiAll *ui, View *vw, Grid *g)
{
	glViewport(0, 0, vw->viewport_size.x, vw->viewport_size.y);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Quad viewport_quad;
	view_viewport_to_quad(vw, &viewport_quad);

	ui_fill_draw(&ui->fill, vw, g, &viewport_quad);
}
