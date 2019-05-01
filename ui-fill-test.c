#include "ui-fill.c"
#include "test.h"
#include "glmock.c"
#include "grid.c"
#include "hex.c"
#include "mesh.c"
#include "quad.c"
#include "shader.c"
#include "ui-grid.c"
#include "view.c"

TEST(ui)
{
	UiGrid *ui_grid = malloc(sizeof *ui_grid);
	UiFill *ui = malloc(sizeof *ui);

	glmock_initialize();
	ui_grid_initialize(ui_grid, 1024);

	_d(ui_fill_initialize(ui, ui_grid));
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
	_s(program->uniforms[ui->uniforms.gridStyles].name);
	//=> gridStyles
	_s(program->uniforms[ui->uniforms.fillColors].name);
	//=> fillColors

	/////////////////////
	// mesh + buffers

	_d(ui->meshes[0].vertices_length);
	//=> 24576
	_d(ui->meshes[1].vertices_length);
	//=> 6144
	_d(ui->meshes[2].vertices_length);
	//=> 1536
	_d(ui->meshes[3].vertices_length);
	//=> 384
	_d(ui->meshes[4].vertices_length);
	//=> 96
	_d(ui->meshes[0].indices_length);
	//=> 49152
	_d(ui->meshes[4].indices_length);
	//=> 192

	_d(ui->buffers[0].vertices);
	//=> 1
	_d(ui->buffers[1].vertices);
	//=> 3
	_d(ui->buffers[2].vertices);
	//=> 5
	_d(ui->buffers[3].vertices);
	//=> 7
	_d(ui->buffers[4].vertices);
	//=> 9
	_d(ui->buffers[0].indices);
	//=> 2
	_d(ui->buffers[4].indices);
	//=> 10

	GLmockBuffer *vertices_buffer = &GLmock.buffers[ui->buffers[0].vertices];
	GLmockBuffer *indices_buffer = &GLmock.buffers[ui->buffers[1].indices];

	_d(vertices_buffer->created);
	//=> 1
	_d(vertices_buffer->size);
	//=> 294912
	_d(vertices_buffer->data == ui->meshes[0].vertices);
	//=> 1
	_d(vertices_buffer->usage == GL_STATIC_DRAW);
	//=> 1

	_d(indices_buffer->created);
	//=> 1
	_d(indices_buffer->size);
	//=> 24576
	_d(indices_buffer->data == ui->meshes[1].indices);
	//=> 1
	_d(indices_buffer->usage == GL_STATIC_DRAW);
	//=> 1

	////////////////////////////
	// terminate

	ui_fill_terminate(ui);

	_d(vertex->deleted);
	//=> 1
	_d(program->deleted);
	//=> 1

	_d(vertices_buffer->deleted);
	//=> 1
	_d(indices_buffer->deleted);
	//=> 1

	ui_grid_terminate(ui_grid);
	free(ui_grid);
	free(ui);
}

TEST(ui_initialize_fail)
{
	UiGrid *ui_grid = malloc(sizeof *ui_grid);
	UiFill *ui = malloc(sizeof *ui);

	// Failures

	glmock_initialize();
	ui_grid_initialize(ui_grid, 1024);
	GLmock.shaders[ui_grid->fragment_shader + 1].compiled = -1;

	_d(ui_fill_initialize(ui, ui_grid));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error compiling shader. Nothing in info log.
	//=>

	ui_grid_terminate(ui_grid);
	glmock_initialize();
	ui_grid_initialize(ui_grid, 1024);
	GLmock.programs[1].linked = -1;

	_d(ui_fill_initialize(ui, ui_grid));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error linking program. Nothing in info log.
	//=>

	ui_grid_terminate(ui_grid);
	free(ui_grid);
	free(ui);
}

TEST(ui_draw_fill)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {100, 100},
		.scale = 10.0,
	};
	Quad quad = {{-126, 1}, {253, 126}};

	Grid *g = malloc(sizeof *g);
	UiGrid *ui_grid = malloc(sizeof *ui_grid);
	UiFill *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &quad);
	ui_grid_initialize(ui_grid, 0);
	ui_grid_update_styles(ui_grid, g);
	ui_fill_initialize(ui, ui_grid);

	Quad viewport_quad;
	view_viewport_to_quad(&vw, &viewport_quad);
	_qd(viewport_quad);
	//=> (-93, -27), (139, 54)

	ui_fill_draw(ui, &vw, g, &viewport_quad);

	_dd(GLmock.using_program, ui->shader.program);
	//=> 1, 1

	_d(GLmock.bound_buffers[0] == ui->buffers[0].vertices);
	//=> 1
	_d(GLmock.bound_buffers[1] == ui->buffers[0].indices);
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
	_d(gridPosition->type == GL_BYTE);
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

	_gg(grid_size->fv0, grid_size->fv1);
	//=> 192, 128
	_hx(g->storage_quad.size);
	//=> 192, 128

	_gg(grid_position_offset->fv0, grid_position_offset->fv1);
	//=> 81, 0

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

	_d(GLmock.draw_elements_mode == GL_TRIANGLES);
	//=> 1
	_d(GLmock.draw_elements_type == GL_UNSIGNED_SHORT);
	//=> 1
	_d(GLmock.draw_elements_count);
	//=> 98304
	_d(49152 * 2);  // Draw the two showing blocks
	//=> 98304

	mat4 *m = &ui_grid->view_matrix;
	_ggg(m->m[3][0], m->m[3][1], m->m[3][3]);
	//=> 0.00944486, 0.0333333, 0.1

	grid_terminate(g);
	ui_grid_terminate(ui_grid);
	ui_fill_terminate(ui);

	free(g);
	free(ui_grid);
	free(ui);
}
