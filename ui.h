#ifndef __UI_H__
#define __UI_H__

#include <GLES2/gl2.h>
#include "mesh.h"
#include "view.h"
#include "grid.h"
#include "quad.h"

typedef struct {
	GLint position;
	GLint gridPosition;
	GLint viewMatrix;
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
} Ui;

void ui_initialize(Ui *ui, Grid *g);
void ui_terminate(Ui *ui);
void ui_print_gl_error(const char *filename, int line);
void ui_draw(Ui *ui, View *vw);

#endif // __UI_H__
