#ifndef __UI_POINTS_H__
#define __UI_POINTS_H__

#include "sixcode.h"
#include <GLES3/gl3.h>
#include "grid.h"
#include "mesh.h"
#include "quad.h"
#include "shader.h"
#include "ui-grid.h"
#include "view.h"

typedef struct {
	GLint position;
	GLint gridPosition;
} UiPointsAttributes;

typedef struct {
	GLint viewMatrix;
	GLint gridSize;
	GLint gridPositionOffset;
	GLint pointSize;
	GLint gridStyles;
	GLint fillColors;
} UiPointsUniforms;

typedef struct {
	GLuint vertices;
} UiPointsBuffers;

typedef struct {
	UiGrid *ui_grid;

	ShaderProgram shader;

	UiPointsAttributes attributes;
	UiPointsUniforms uniforms;
	UiPointsBuffers buffers;

	PointsMesh mesh;
} UiPoints;

i8 ui_points_initialize(UiPoints *ui, UiGrid *ui_grid);
void ui_points_terminate(UiPoints *ui);
void ui_points_draw(UiPoints *ui, View *vw, Grid *g, Quad *viewport_quad);

#endif // __UI_POINTS_H__
