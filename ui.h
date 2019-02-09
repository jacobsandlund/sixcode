#ifndef __UI_H__
#define __UI_H__

#include <GLES2/gl2.h>
#include "mesh.h"
#include "view.h"
#include "grid.h"
#include "shader.h"
#include "sixcode.h"
#include "quad.h"

#define UI_MAX_TEXTURE_SIZE 4096

#define UI_MESH_MAX_SIZE 64
#define UI_NUM_MESHES 4     // 64, 32, 16, 8

typedef struct {
	GLint position;
	GLint gridPosition;
	GLint gridPosition2;
} UiAttributes;

typedef struct {
	GLint viewMatrix;
	GLint gridSize;
	GLint gridPositionOffset;
	GLint strokeColor;
	GLint fillColors;
	GLint gridStyles;
} UiUniforms;

typedef struct {
	GLuint fill_vertices;
	GLuint fill_indices;
	GLuint stroke_vertices;
} UiBuffers;

typedef struct {
	GLuint fill_colors;
	GLuint grid_styles;
} UiTextures;

typedef struct {
	ShaderProgram fill_shader;
	ShaderProgram stroke_shader;

	UiAttributes attributes;
	UiUniforms fill_uniforms;
	UiUniforms stroke_uniforms;
	UiBuffers buffers[UI_NUM_MESHES];
	UiTextures textures;

	Mesh meshes[UI_NUM_MESHES];

	mat4 view_matrix;
	f64 translation_x;
	f64 translation_y;

	u8 *styles_buffer;
	i32 styles_buffer_capacity;
	i32 styles_buffer_capacity_max;
} Ui;

i8 ui_initialize(Ui *ui, i32 styles_buffer_capacity_max);
void ui_terminate(Ui *ui);
void ui_draw_fill(Ui *ui, View *vw, Grid *g, Quad *viewport_quad);
void ui_draw_stroke(Ui *ui, View *vw, Grid *g, Quad *viewport_quad);
void ui_update_styles(Ui *ui, Grid *g);
void ui_update_styles_in_quad(Ui *ui, Grid *g, Quad *quad);

#endif // __UI_H__
