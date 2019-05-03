#ifndef __UI_GRID_H__
#define __UI_GRID_H__

#include "sixcode.h"
#include <GLES3/gl3.h>
#include "grid.h"
#include "quad.h"
#include "shader.h"
#include "view.h"

#define UI_GRID_MAX_TEXTURE_SIZE 4096

typedef struct {
	GLuint grid_styles;
	GLuint fill_colors;
} UiGridTextures;

typedef struct {
	GLuint fragment_shader;
	UiGridTextures textures;

	u8 *styles_buffer;
	int styles_buffer_capacity;
	int styles_buffer_capacity_max;
} UiGrid;

bool ui_grid_initialize(UiGrid *ui, int styles_buffer_capacity_max);
void ui_grid_terminate(UiGrid *ui);
void ui_grid_size_quad_for_draw(SizeQuad *out_sq, Quad *grid_styles_quad, Quad *viewport_quad);
void ui_grid_update_styles(UiGrid *ui, Grid *g);
void ui_grid_update_styles_in_quad(UiGrid *ui, Grid *g, Quad *quad);

#endif // __UI_GRID_H__
