#include <stdio.h>
#include <string.h>
#include "ui-grid.h"

#define UI_GRID_FILL_COLORS_COUNT 256
#define UI_GRID_FILL_COLOR_COMPONENTS_LENGTH 768  // 256 * 3

#define UI_GRID_STYLES_BUFFER_CAPACITY_MIN 256

const char UI_GRID_FRAGMENT_SHADER_SOURCE[] =
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	gl_FragColor = color;\n"
"}\n";

const u8 UI_GRID_FILL_COLORS[UI_GRID_FILL_COLOR_COMPONENTS_LENGTH] = {

	// Area middles (0 - 31)

	// 244, 244, 244,  // grey - nothing
	255, 255, 255,  // white - nothing
	237, 243, 252,  // blue
	239, 252, 237,	// green
	252, 237, 239,	// red

	227, 252, 252,	// aqua
	246, 234, 252,  // purple
	252, 251, 224,	// yellow
	255, 247, 232,	// orange

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// Area edges (32 - 63)

	// 186, 186, 186,  // grey - nothing
	255, 255, 255,  // white - nothing
	92, 150, 237,	// blue
	49, 163, 24,	// green
	255, 224, 228,	// red

	22, 186, 186,	// aqua
	175, 44, 232,	// purple
	186, 183, 1,	// yellow
	216, 105, 2,	// orange

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// Area middles - zoomed out (64 - 95)

	// 244, 244, 244,  // grey - nothing
	255, 255, 255,  // white - nothing
	237, 243, 252,  // blue
	239, 252, 237,	// green
	255, 224, 228,	// red

	227, 252, 252,	// aqua
	246, 234, 252,  // purple
	252, 251, 224,	// yellow
	255, 247, 232,	// orange

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// Area edges - zoomed out (96 - 127)

	// 186, 186, 186,  // grey - nothing
	255, 255, 255,  // white - nothing
	92, 150, 237,	// blue
	49, 163, 24,	// green
	242, 188, 195,	// red

	22, 186, 186,	// aqua
	175, 44, 232,	// purple
	186, 183, 1,	// yellow
	216, 105, 2,	// orange

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// Hex fills (128 - 191)

	64, 239, 233,
	190, 190, 190,
	255, 140, 140,
	140, 255, 140,
	
	140, 140, 255,
	255, 255, 40,
	255, 40, 255,
	40, 255, 255,
	
	255, 190, 90,
	255, 90, 190,
	190, 255, 90,
	90, 255, 190,
	
	190, 90, 255,
	90, 190, 255,
	220, 190, 140,
	140, 190, 220,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// ...

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// Hex fills - zoomed out (192 - 255)

	64, 239, 233,
	190, 190, 190,
	255, 140, 140,
	140, 255, 140,
	
	140, 140, 255,
	255, 255, 40,
	255, 40, 255,
	40, 255, 255,
	
	255, 190, 90,
	255, 90, 190,
	190, 255, 90,
	90, 255, 190,
	
	190, 90, 255,
	90, 190, 255,
	220, 190, 140,
	140, 190, 220,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// ...

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

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

	glGenTextures(1, &ui->textures.fill_colors);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ui->textures.fill_colors);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			UI_GRID_FILL_COLORS_COUNT,
			1,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			UI_GRID_FILL_COLORS);

	///////////////////////
	// styles_buffer

	if (styles_buffer_capacity_max < UI_GRID_STYLES_BUFFER_CAPACITY_MIN) {
		styles_buffer_capacity_max = UI_GRID_STYLES_BUFFER_CAPACITY_MIN;
	}

	ui->styles_buffer = malloc(UI_GRID_STYLES_BUFFER_CAPACITY_MIN * sizeof *ui->styles_buffer);
	ui->styles_buffer_capacity = UI_GRID_STYLES_BUFFER_CAPACITY_MIN;
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
	glDeleteTextures(1, &ui->textures.fill_colors);

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

void ui_grid_size_quad_for_draw(SizeQuad *out_sq, Quad *grid_styles_quad, Quad *viewport_quad)
{
	Quad intersect_quad;
	quad_intersect(&intersect_quad, viewport_quad, grid_styles_quad);
	quad_to_storage_space_size_quad(out_sq, &intersect_quad);
	size_quad_even_align(out_sq, out_sq);
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
	SizeQuad sq;
	quad_to_storage_space_size_quad(&sq, quad);
	i32 need_capacity = size_quad_capacity(&sq);

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
