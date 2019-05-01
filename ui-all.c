#include "ui-all.h"

#define UI_ALL_NUM_COMPONENTS 1

i8 ui_all_initialize(UiAll *ui, i32 styles_buffer_capacity_max)
{
	if (!ui_grid_initialize(&ui->grid, styles_buffer_capacity_max)) {
		return 0;
	}

	i8 initialized[UI_ALL_NUM_COMPONENTS] = {
		[0] = ui_fill_initialize(&ui->fill, &ui->grid),
	};

	i32 count = 0;
	for (i32 i = 0; i < UI_ALL_NUM_COMPONENTS; ++i) {
		if (initialized[i]) {
			++count;
		}
	}

	if (count < UI_ALL_NUM_COMPONENTS) {

		// Indices need to match above
		if (initialized[0]) ui_fill_terminate(&ui->fill);

		ui_grid_terminate(&ui->grid);

		return 0;
	}

	return 1;
}

void ui_all_terminate(UiAll *ui)
{
	ui_fill_terminate(&ui->fill);
	ui_grid_terminate(&ui->grid);
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
