#ifndef __UI_H__
#define __UI_H__

#include <GLES2/gl2.h>
#include "mesh.h"
#include "view.h"
#include "grid.h"
#include "sixcode.h"
#include "quad.h"

#define UI_HEX_MESH_SIZE GRID_BLOCK_SIZE_R
#define UI_MAX_TEXTURE_SIZE 4096

typedef struct {
	GLint position;
	GLint gridPosition;

	GLint viewMatrix;
	GLint gridSize;
	GLint gridPositionOffset;

	GLint fillColors;
	GLint gridStyles;
} UiLocations;

typedef struct {
	GLuint hex_mesh_vertices;
	GLuint hex_mesh_fill_indices;
	GLuint hex_mesh_stroke_indices;
} UiBuffers;

typedef struct {
	GLuint fill_colors;
	GLuint grid_styles;
} UiTextures;

typedef struct {
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	UiLocations locations;
	UiBuffers buffers;
	UiTextures textures;

	Mesh hex_mesh;

	mat4 view_matrix;

	u8 *styles_buffer;
	i32 styles_buffer_capacity;
	i32 styles_buffer_capacity_max;
} Ui;

i8 ui_initialize(Ui *ui, i32 styles_buffer_capacity_max);
void ui_terminate(Ui *ui);
void ui_print_gl_error(const char *filename, int line);
void ui_draw(Ui *ui, View *vw, Grid *g);
void ui_update_styles(Ui *ui, Grid *g);
void ui_update_styles_in_quad(Ui *ui, Grid *g, Quad *quad);

#endif // __UI_H__
