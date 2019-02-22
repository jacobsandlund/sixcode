#ifndef __UI_FILL_H__
#define __UI_FILL_H__

#include <GLES2/gl2.h>
#include "mesh.h"
#include "view.h"
#include "grid.h"
#include "shader.h"
#include "sixcode.h"
#include "quad.h"
#include "ui-grid.h"

#define UI_FILL_MESH_MAX_SIZE 64
#define UI_FILL_NUM_MESHES 5	// 64, 32, 16, 8, 4

typedef struct {
	GLint position;
	GLint gridPosition;
} UiFillAttributes;

typedef struct {
	GLint viewMatrix;
	GLint gridSize;
	GLint gridPositionOffset;
	GLint styleOffset;
	GLint gridStyles;
	GLint fillColors;
} UiFillUniforms;

typedef struct {
	GLuint vertices;
	GLuint indices;
} UiFillBuffers;

typedef struct {
	UiGrid *ui_grid;

	ShaderProgram shader;

	UiFillAttributes attributes;
	UiFillUniforms uniforms;
	UiFillBuffers buffers[UI_FILL_NUM_MESHES];

	FillMesh meshes[UI_FILL_NUM_MESHES];
} UiFill;

i8 ui_fill_initialize(UiFill *ui, UiGrid *ui_grid);
void ui_fill_terminate(UiFill *ui);
void ui_fill_draw(UiFill *ui, View *vw, Grid *g, Quad *viewport_quad);

#endif // __UI_FILL_H__
