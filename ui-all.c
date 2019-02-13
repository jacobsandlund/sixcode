#include "ui-all.h"

#define UI_ALL_FILL_SCALE_CUTOFF 4.0f
#define UI_ALL_STROKE_SCALE_CUTOFF 8.0f

#define UI_ALL_NUM_COMPONENTS 4

i8 ui_all_initialize(UiAll *ui, i32 styles_buffer_capacity_max)
{
	i8 initialized[UI_ALL_NUM_COMPONENTS] = {
		[0] = ui_grid_initialize(&ui->grid, styles_buffer_capacity_max),
		[1] = ui_fill_initialize(&ui->fill, &ui->grid),
		[2] = ui_points_initialize(&ui->points, &ui->grid),
		[3] = ui_stroke_initialize(&ui->stroke, &ui->grid),
	};

	i32 count = 0;
	for (i32 i = 0; i < UI_ALL_NUM_COMPONENTS; ++i) {
		if (initialized[i]) {
			++count;
		}
	}

	if (count < UI_ALL_NUM_COMPONENTS) {

		// Indices need to match above
		if (initialized[0]) ui_grid_terminate(&ui->grid);
		if (initialized[1]) ui_fill_terminate(&ui->fill);
		if (initialized[2]) ui_points_terminate(&ui->points);
		if (initialized[3]) ui_stroke_terminate(&ui->stroke);

		return 0;
	}

	return 1;
}

void ui_all_terminate(UiAll *ui)
{
	ui_grid_terminate(&ui->grid);
	ui_fill_terminate(&ui->fill);
	ui_points_terminate(&ui->points);
	ui_stroke_terminate(&ui->stroke);
}

void ui_all_draw(UiAll *ui, View *vw, Grid *g)
{
	glViewport(0, 0, vw->viewport_size.x, vw->viewport_size.y);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Quad viewport_quad;
	view_viewport_to_quad(vw, &viewport_quad);

	if (vw->scale >= UI_ALL_FILL_SCALE_CUTOFF) {
		ui_fill_draw(&ui->fill, vw, g, &viewport_quad);
	} else {
		ui_points_draw(&ui->points, vw, g, &viewport_quad);
	}

	if (vw->scale >= UI_ALL_STROKE_SCALE_CUTOFF) {
		ui_stroke_draw(&ui->stroke, vw, g, &viewport_quad);
	}
}
