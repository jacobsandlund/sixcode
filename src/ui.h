#ifndef Ui_h
#define Ui_h

#include "spacetime.h"
#include <GLES3/gl3.h>
#include "grid.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "viewport.h"

#define UI_MESH_MAX_SIZE 8
#define UI_NUM_MESHES 4	// 8, 4, 2, 1

typedef struct {
	GLint position;
	GLint gridPosition;
	GLint positionOffset;
	GLint gridPositionOffset;
} UiAttributes;

typedef struct {
	GLint viewMatrix;
	GLint gridSize;
	GLint styleOffset;
	GLint gridStyles;
	GLint fillColors;
} UiUniforms;

typedef struct {
	GLuint vertices;
	GLuint indices;
} UiBuffers;

typedef struct {
	UiBuffers buffers[UI_NUM_MESHES];
	FillMesh meshes[UI_NUM_MESHES];
} UiLayoutData;

typedef struct {
	ShaderProgram shader;
	Texture grid_styles_texture;
	Texture fill_colors_texture;

	UiAttributes attributes;
	UiUniforms uniforms;
	GLuint instanceBuffer;

	UiLayoutData layouts[camera_NUM_LAYOUTS];
	InstanceMesh instance_mesh;
} Ui;

bool ui_init(Ui *ui);
void ui_destroy(Ui *ui);
void ui_draw(Ui *ui, Camera *c, Grid *g);

#endif // Ui_h
