#include <stdio.h>
#include <string.h>
#include "ui-grid.h"

#define UI_STYLES_BUFFER_CAPACITY_MIN 256

const char UI_GRID_FRAGMENT_SHADER_SOURCE[] =
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	gl_FragColor = color;\n"
"}\n";

i8 ui_grid_initialize(UiGrid *ui, i32 styles_buffer_capacity_max)
{
	////////////////////////
	// load/create

	ui->fragment_shader = shader_load(GL_FRAGMENT_SHADER, UI_GRID_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);

	if (!ui->fragment_shader) {
		return 0;
	}

	////////////////
	// textures

	glGenTextures(1, &ui->textures.grid_styles);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui->textures.grid_styles);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	///////////////////////
	// styles_buffer

	if (styles_buffer_capacity_max < UI_STYLES_BUFFER_CAPACITY_MIN) {
		styles_buffer_capacity_max = UI_STYLES_BUFFER_CAPACITY_MIN;
	}

	ui->styles_buffer = malloc(UI_STYLES_BUFFER_CAPACITY_MIN * sizeof *ui->styles_buffer);
	ui->styles_buffer_capacity = UI_STYLES_BUFFER_CAPACITY_MIN;
	ui->styles_buffer_capacity_max = styles_buffer_capacity_max;

	///////////////////////
	// view_matrix

	for (i32 i = 0; i < 4; i++) {
		for (i32 j = 0; j < 4; j++) {
			ui->view_matrix.m[i][j] = 0.0f;
		}
	}

	return 1;
}

void ui_grid_terminate(UiGrid *ui)
{
	glDeleteShader(ui->fragment_shader);
	glDeleteTextures(1, &ui->textures.grid_styles);

	free(ui->styles_buffer);
}

void ui_grid_update_view_matrix(UiGrid *ui, View *vw)
{
	f64 scale_inv = 1.0 / (f64) vw->scale;
	ui->translation_x = -vw->translation.x * scale_inv * 2.0;
	ui->translation_y = vw->translation.y * scale_inv * 2.0;
	ui->view_matrix.m[0][0] = 1.0 / (f64) vw->viewport_size.x;
	ui->view_matrix.m[1][1] = 1.0 / (f64) vw->viewport_size.y;
	ui->view_matrix.m[3][3] = scale_inv;
}

void ui_grid_storage_quad_for_draw(StorageQuad *out_sq, Quad *grid_styles_quad, Quad *viewport_quad)
{
	Quad intersect_quad;
	quad_intersect(&intersect_quad, viewport_quad, grid_styles_quad);
	storage_quad_from_quad(out_sq, &intersect_quad);
	storage_quad_even_align(out_sq, out_sq);
}

void ui_grid_update_styles(UiGrid *ui, Grid *g)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui->textures.grid_styles);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			g->storage_quad.size.c,
			g->storage_quad.size.r,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			g->styles);
}

void ui_grid_update_styles_in_quad(UiGrid *ui, Grid *g, Quad *quad)
{
	StorageQuad sq;
	storage_quad_from_quad(&sq, quad);
	i32 need_capacity = storage_quad_capacity(&sq);

	if (need_capacity > ui->styles_buffer_capacity) {
		if (need_capacity > ui->styles_buffer_capacity_max) {
			ui_grid_update_styles(ui, g);

			return;
		}

		free(ui->styles_buffer);

		ui->styles_buffer = malloc(need_capacity * sizeof *ui->styles_buffer);
		ui->styles_buffer_capacity = need_capacity;
	}

	Hex min = hex_sub(sq.min, g->storage_quad.min);
	i32 storage_size_c = g->storage_quad.size.c;

	for (i32 r = 0; r < sq.size.r; ++r) {
		u8 *dest = &ui->styles_buffer[r * sq.size.c];
		i32 src_i = (r + min.r) * storage_size_c + min.c;
		const u8 *src = &g->styles[src_i];

		memcpy(dest, src, sq.size.c);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui->textures.grid_styles);

	glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			min.c,
			min.r,
			sq.size.c,
			sq.size.r,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			ui->styles_buffer);
}
