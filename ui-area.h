#ifndef __UI_AREA_H__
#define __UI_AREA_H__

#include <GLES2/gl2.h>
#include "mesh.h"
#include "view.h"
#include "shader.h"
#include "sixcode.h"
#include "quad.h"

typedef struct {
	GLint position;
} UiAreaAttributes;

typedef struct {
	GLint viewMatrix;
	GLint gridSize;
	GLint gridPositionOffset;
	GLint gridStyles;
	GLint fillColors;
} UiAreaUniforms;

typedef struct {
	GLuint vertices;
	GLuint indices;
} UiAreaBuffers;

typedef struct {
	UiGrid *ui_grid;

	ShaderProgram shader;

	UiAreaAttributes attributes;
	UiAreaUniforms uniforms;
	UiAreaBuffers buffers[UI_FILL_NUM_MESHES];

	FillMesh meshes[UI_FILL_NUM_MESHES];
} UiArea;

i8 ui_fill_initialize(UiArea *ui, UiGrid *ui_grid);
void ui_fill_terminate(UiArea *ui);
void ui_fill_draw(UiArea *ui, View *vw, Grid *g, Quad *viewport_quad);

#endif // __UI_AREA_H__
