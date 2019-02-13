#include "test.h"
#include "glmock.c"
#include "grid.c"
#include "hex.c"
#include "mesh.c"
#include "shader.c"
#include "space.c"
#include "quad.c"
#include "ui-grid.c"
#include "ui-points.c"
#include "view.c"

TEST(ui)
{
	UiGrid *ui_grid = malloc(sizeof *ui_grid);
	UiPoints *ui = malloc(sizeof *ui);

	glmock_initialize();
	ui_grid_initialize(ui_grid, 1024);

	_d(ui_points_initialize(ui, ui_grid));
	//=> 1

	///////////////////////
	// load/create/link

	_d(ui->shader.vertex);
	//=> 2
	
	GLmockShader *vertex = &GLmock.shaders[ui->shader.vertex];
	_dd(vertex->created, vertex->compiled);
	//=> 1, 1

	_d(ui->shader.program);
	//=> 1
	
	GLmockProgram *program = &GLmock.programs[ui->shader.program];

	_d(program->created);
	//=> 1
	_d(program->attached_vertex_shader);
	//=> 2
	_d(program->attached_fragment_shader);
	//=> 1

	_d(program->linked);
	//=> 1

	/////////////////////
	// attributes

	_d(ui->attributes.position);
	//=> 1
	_d(ui->attributes.gridPosition);
	//=> 2

	_s(program->attributes[ui->attributes.position].name);
	//=> position
	_s(program->attributes[ui->attributes.gridPosition].name);
	//=> gridPosition

	////////////////////
	// uniforms

	_d(ui->uniforms.viewMatrix);
	//=> 1
	_d(ui->uniforms.gridSize);
	//=> 2
	_d(ui->uniforms.gridPositionOffset);
	//=> 3
	_d(ui->uniforms.pointSize);
	//=> 4
	_d(ui->uniforms.gridStyles);
	//=> 5
	_d(ui->uniforms.fillColors);
	//=> 6

	_s(program->uniforms[ui->uniforms.viewMatrix].name);
	//=> viewMatrix
	_s(program->uniforms[ui->uniforms.gridSize].name);
	//=> gridSize
	_s(program->uniforms[ui->uniforms.gridPositionOffset].name);
	//=> gridPositionOffset
	_s(program->uniforms[ui->uniforms.pointSize].name);
	//=> pointSize
	_s(program->uniforms[ui->uniforms.gridStyles].name);
	//=> gridStyles
	_s(program->uniforms[ui->uniforms.fillColors].name);
	//=> fillColors

	/////////////////////
	// mesh + buffers

	_d(ui->mesh.vertices_length);
	//=> 65536

	_d(ui->buffers.vertices);
	//=> 1

	GLmockBuffer *vertices_buffer = &GLmock.buffers[ui->buffers.vertices];

	_d(vertices_buffer->created);
	//=> 1
	_d(vertices_buffer->size);
	//=> 786432
	_d(vertices_buffer->data == ui->mesh.vertices);
	//=> 1
	_d(vertices_buffer->usage == GL_STATIC_DRAW);
	//=> 1

	////////////////////////////
	// terminate

	ui_points_terminate(ui);

	_d(vertex->deleted);
	//=> 1

	_d(vertices_buffer->deleted);
	//=> 1

	ui_grid_terminate(ui_grid);
	free(ui_grid);
	free(ui);
}

TEST(ui_initialize_fail)
{
	UiGrid *ui_grid = malloc(sizeof *ui_grid);
	UiPoints *ui = malloc(sizeof *ui);

	// Failures

	glmock_initialize();
	ui_grid_initialize(ui_grid, 1024);
	GLmock.shaders[ui_grid->fragment_shader + 1].compiled = -1;

	_d(ui_points_initialize(ui, ui_grid));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error compiling shader. Nothing in info log.
	//=>

	ui_grid_terminate(ui_grid);
	glmock_initialize();
	ui_grid_initialize(ui_grid, 1024);
	GLmock.programs[1].linked = -1;

	_d(ui_points_initialize(ui, ui_grid));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error linking program. Nothing in info log.
	//=>

	ui_grid_terminate(ui_grid);
	free(ui_grid);
	free(ui);
}

TEST(ui_points_draw)
{
	View vw = {
		.viewport_size = {1600, 1000},
		.translation = {-600, -300},
		.scale = 1.0,
	};
	Quad quad = {{-826, -298}, {753, 826}};

	Grid *g = malloc(sizeof *g);
	UiGrid *ui_grid = malloc(sizeof *ui_grid);
	UiPoints *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &quad);
	ui_grid_initialize(ui_grid, 0);
	ui_grid_update_styles(ui_grid, g);
	ui_points_initialize(ui, ui_grid);

	Quad viewport_quad;
	view_viewport_to_quad(&vw, &viewport_quad);
	_qd(viewport_quad);
	//=> (-3236, -1068), (465, 268)

	ui_points_draw(ui, &vw, g, &viewport_quad);

	_dd(GLmock.using_program, ui->shader.program);
	//=> 1, 1

	_d(GLmock.bound_buffers[0] == ui->buffers.vertices);
	//=> 1

	// Attributes

	GLmockProgram *program = &GLmock.programs[ui->shader.program];

	GLmockAttribute *position = &program->attributes[ui->attributes.position];
	GLmockAttribute *gridPosition = &program->attributes[ui->attributes.gridPosition];

	_d(position->size);
	//=> 2
	_d(position->type == GL_FLOAT);
	//=> 1
	_d(position->stride);
	//=> 12
	_d(position->offset);
	//=> 0
	_d(position->enabled_vertex_attrib_array);
	//=> 1
	_gggg(position->v0, position->v1, position->v2, position->v3);
	//=> 0, 0, 0, 1

	_d(gridPosition->size);
	//=> 2
	_d(gridPosition->type == GL_SHORT);
	//=> 1
	_d(gridPosition->stride);
	//=> 12
	_d(gridPosition->offset);
	//=> 8
	_d(gridPosition->enabled_vertex_attrib_array);
	//=> 1

	// Uniform

	GLmockUniform *grid_size = &program->uniforms[ui->uniforms.gridSize];
	GLmockUniform *grid_position_offset = &program->uniforms[ui->uniforms.gridPositionOffset];
	GLmockUniform *point_size = &program->uniforms[ui->uniforms.pointSize];

	_gg(grid_size->fv0, grid_size->fv1);
	//=> 832, 1152
	_hx(g->storage_quad.size);
	//=> 832, 1152

	_g(point_size->fv0);
	//=> 0.75

	_gg(grid_position_offset->fv0, grid_position_offset->fv1);
	//=> 512, 512

	_d(program->uniforms[ui->uniforms.viewMatrix].matrix4fv == &ui_grid->view_matrix.m[0][0]);
	//=> 1

	// Textures

	_d(program->uniforms[ui->uniforms.gridStyles].iv0);
	//=> 0
	_d(program->uniforms[ui->uniforms.fillColors].iv0);
	//=> 1

	_d(GLmock.bound_textures[0] == ui_grid->textures.grid_styles);
	//=> 1
	_d(GLmock.bound_textures[1] == ui_grid->textures.fill_colors);
	//=> 1

	// Draw

	_d(GLmock.draw_arrays_mode == GL_POINTS);
	//=> 1
	_d(GLmock.draw_arrays_count);
	//=> 589824
	_d(ui->mesh.vertices_length * 9);  // Draw 9 point blocks
	//=> 589824

	mat4 *m = &ui_grid->view_matrix;
	_gggg(m->m[3][0], m->m[3][1], m->m[3][2], m->m[3][3]);
	//=> 0.819282, -0.888, 0, 1

	grid_terminate(g);
	ui_grid_terminate(ui_grid);
	ui_points_terminate(ui);

	free(g);
	free(ui_grid);
	free(ui);
}
