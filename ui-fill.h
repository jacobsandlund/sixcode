#ifndef __UI_FILL_H__
#define __UI_FILL_H__

#include "sixcode.h"
#include <GLES3/gl3.h>
#include "grid.h"
#include "mesh.h"
#include "quad.h"
#include "shader.h"
#include "ui-grid.h"
#include "view.h"

#define UI_FILL_MESH_MAX_SIZE 8
#define UI_FILL_NUM_MESHES 4	// 8, 4, 2, 1

typedef struct {
	GLint position;
	GLint gridPosition;
	GLint positionOffset;
	GLint gridPositionOffset;
} UiFillAttributes;

typedef struct {
	GLint viewMatrix;
	GLint gridSize;
	GLint styleOffset;
	GLint gridStyles;
	GLint fillColors;
} UiFillUniforms;

typedef struct {
	GLuint vertices;
	GLuint indices;
} UiFillBuffers;

typedef struct {
	UiFillBuffers buffers[UI_FILL_NUM_MESHES];
	FillMesh meshes[UI_FILL_NUM_MESHES];
} UiFillLayoutData;

typedef struct {
	UiGrid *ui_grid;

	ShaderProgram shader;

	UiFillAttributes attributes;
	UiFillUniforms uniforms;
	GLuint instanceBuffer;

	UiFillLayoutData layouts[VIEW_NUM_LAYOUTS];
	InstanceMesh instance_mesh;
} UiFill;

bool ui_fill_initialize(UiFill *ui, UiGrid *ui_grid);
void ui_fill_terminate(UiFill *ui);
void ui_fill_draw(UiFill *ui, View *vw, Grid *g, Quad *viewport_quad);

#endif // __UI_FILL_H__
