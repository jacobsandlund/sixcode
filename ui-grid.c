#include "ui-grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	250, 250, 250,
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
};

bool ui_grid_initialize(UiGrid *ui, int styles_buffer_capacity_max)
{
	////////////////////////
	// load/create

	ui->fragment_shader = shader_load(GL_FRAGMENT_SHADER, UI_GRID_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);

	if (!ui->fragment_shader) {
		return false;
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

	return true;
}

void ui_grid_terminate(UiGrid *ui)
{
	glDeleteShader(ui->fragment_shader);
	glDeleteTextures(1, &ui->textures.grid_styles);
	glDeleteTextures(1, &ui->textures.fill_colors);

	free(ui->styles_buffer);
}

void ui_grid_size_quad_for_draw(SizeQuad *out_sq, Quad *grid_styles_quad, Quad *viewport_quad)
{
	Quad intersect_quad;
	quad_intersect(&intersect_quad, viewport_quad, grid_styles_quad);
	quad_to_size_quad(out_sq, &intersect_quad);
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
			g->size_quad.size.x,
			g->size_quad.size.y,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			g->styles);
}

void ui_grid_update_styles_in_quad(UiGrid *ui, Grid *g, Quad *quad)
{
	SizeQuad sq;
	quad_to_size_quad(&sq, quad);
	int need_capacity = size_quad_capacity(&sq);

	if (need_capacity > ui->styles_buffer_capacity) {
		if (need_capacity > ui->styles_buffer_capacity_max) {
			ui_grid_update_styles(ui, g);

			return;
		}

		free(ui->styles_buffer);

		ui->styles_buffer = malloc(need_capacity * sizeof *ui->styles_buffer);
		ui->styles_buffer_capacity = need_capacity;
	}

	ivec2 min = ivec2_sub(sq.min, g->size_quad.min);
	int storage_size_x = g->size_quad.size.x;

	for (int r = 0; r < sq.size.y; ++r) {
		u8 *dest = &ui->styles_buffer[r * sq.size.x];
		int src_i = (r + min.y) * storage_size_x + min.x;
		const u8 *src = &g->styles[src_i];

		memcpy(dest, src, sq.size.x);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui->textures.grid_styles);

	glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			min.x,
			min.y,
			sq.size.x,
			sq.size.y,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			ui->styles_buffer);
}
