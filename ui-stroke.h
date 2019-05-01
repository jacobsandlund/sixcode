#ifndef __UI_STROKE_H__
#define __UI_STROKE_H__

#include "sixcode.h"
#include <GLES3/gl3.h>
#include "grid.h"
#include "mesh.h"
#include "quad.h"
#include "shader.h"
#include "ui-grid.h"
#include "view.h"

#define UI_STROKE_MESH_MAX_SIZE 64
#define UI_STROKE_NUM_MESHES 5     // 64, 32, 16, 8, 4

typedef struct {
	GLint position;
	GLint gridPosition;
	GLint gridPosition2;
} UiStrokeAttributes;

typedef struct {
	GLint viewMatrix;
	GLint gridSize;
	GLint gridPositionOffset;
	GLint strokeColor;
	GLint gridStyles;
	GLint fillColors;
} UiStrokeUniforms;

typedef struct {
	GLuint vertices;
} UiStrokeBuffers;

typedef struct {
	UiGrid *ui_grid;

	ShaderProgram shader;

	UiStrokeAttributes attributes;
	UiStrokeUniforms uniforms;
	UiStrokeBuffers buffers[UI_STROKE_NUM_MESHES];

	StrokeMesh meshes[UI_STROKE_NUM_MESHES];
} UiStroke;

i8 ui_stroke_initialize(UiStroke *ui, UiGrid *ui_grid);
void ui_stroke_terminate(UiStroke *ui);
void ui_stroke_draw(UiStroke *ui, View *vw, Grid *g, Quad *viewport_quad);

#endif // __UI_STROKE_H__
