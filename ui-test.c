#include "test.h"
#include "glmock.c"
#include "grid.c"
#include "hex.c"
#include "matrix.c"
#include "mesh.c"
#include "shader.c"
#include "quad.c"
#include "ui.c"
#include "view.c"

TEST(ui)
{
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();

	_d(ui_initialize(ui, 1024));
	//=> 1

	///////////////////////
	// load/create

	_d(ui->fill_shader.vertex);
	//=> 1
	_dd(ui->fill_shader.fragment, ui->stroke_shader.fragment);
	//=> 2, 2
	_d(ui->stroke_shader.vertex);
	//=> 3
	
	GLmockShader *fill_vertex = &GLmock.shaders[ui->fill_shader.vertex];
	GLmockShader *fragment = &GLmock.shaders[ui->fill_shader.fragment];
	GLmockShader *stroke_vertex = &GLmock.shaders[ui->stroke_shader.vertex];
	_dd(fill_vertex->created, fill_vertex->compiled);
	//=> 1, 1
	_dd(stroke_vertex->created, stroke_vertex->compiled);
	//=> 1, 1
	_dd(fragment->created, fragment->compiled);
	//=> 1, 1

	_d(ui->fill_shader.program);
	//=> 1
	_d(ui->stroke_shader.program);
	//=> 2
	
	GLmockProgram *fill_program = &GLmock.programs[ui->fill_shader.program];
	GLmockProgram *stroke_program = &GLmock.programs[ui->stroke_shader.program];

	_d(fill_program->created);
	//=> 1
	_d(fill_program->attached_vertex_shader);
	//=> 1
	_d(fill_program->attached_fragment_shader);
	//=> 2

	_d(stroke_program->created);
	//=> 1
	_d(stroke_program->attached_vertex_shader);
	//=> 3
	_d(stroke_program->attached_fragment_shader);
	//=> 2

	/////////////////////
	// attributes

	_d(ui->attributes.position);
	//=> 0
	_d(ui->attributes.gridPosition);
	//=> 1
	_d(ui->attributes.gridPosition2);
	//=> 2

	_s(fill_program->attributes[ui->attributes.position].name);
	//=> position
	_s(fill_program->attributes[ui->attributes.gridPosition].name);
	//=> gridPosition
	_s(stroke_program->attributes[ui->attributes.position].name);
	//=> position
	_s(stroke_program->attributes[ui->attributes.gridPosition].name);
	//=> gridPosition
	_s(stroke_program->attributes[ui->attributes.gridPosition2].name);
	//=> gridPosition2

	/////////////////////
	// link

	_d(fill_program->linked);
	//=> 1
	_d(stroke_program->linked);
	//=> 1

	////////////////////
	// uniforms

	_d(ui->fill_uniforms.viewMatrix);
	//=> 1
	_d(ui->fill_uniforms.gridSize);
	//=> 2
	_d(ui->fill_uniforms.gridPositionOffset);
	//=> 3
	_d(ui->fill_uniforms.fillColors);
	//=> 4
	_d(ui->fill_uniforms.gridStyles);
	//=> 5

	_d(ui->stroke_uniforms.viewMatrix);
	//=> 1
	_d(ui->stroke_uniforms.gridSize);
	//=> 2
	_d(ui->stroke_uniforms.gridPositionOffset);
	//=> 3
	_d(ui->stroke_uniforms.strokeColor);
	//=> 4
	_d(ui->stroke_uniforms.gridStyles);
	//=> 5

	_s(fill_program->uniforms[ui->fill_uniforms.viewMatrix].name);
	//=> viewMatrix
	_s(fill_program->uniforms[ui->fill_uniforms.gridSize].name);
	//=> gridSize
	_s(fill_program->uniforms[ui->fill_uniforms.gridPositionOffset].name);
	//=> gridPositionOffset
	_s(fill_program->uniforms[ui->fill_uniforms.fillColors].name);
	//=> fillColors
	_s(fill_program->uniforms[ui->fill_uniforms.gridStyles].name);
	//=> gridStyles

	_s(stroke_program->uniforms[ui->stroke_uniforms.viewMatrix].name);
	//=> viewMatrix
	_s(stroke_program->uniforms[ui->stroke_uniforms.gridSize].name);
	//=> gridSize
	_s(stroke_program->uniforms[ui->stroke_uniforms.gridPositionOffset].name);
	//=> gridPositionOffset
	_s(stroke_program->uniforms[ui->stroke_uniforms.strokeColor].name);
	//=> strokeColor
	_s(stroke_program->uniforms[ui->stroke_uniforms.gridStyles].name);
	//=> gridStyles

	/////////////////////
	// mesh + buffers

	_d(ui->mesh.fill_vertices_length);
	//=> 24576
	_d(ui->mesh.fill_indices_length);
	//=> 49152
	_d(ui->mesh.stroke_vertices_length);
	//=> 24576

	_d(ui->buffers.fill_vertices);
	//=> 1
	_d(ui->buffers.fill_indices);
	//=> 2
	_d(ui->buffers.stroke_vertices);
	//=> 3

	GLmockBuffer *vertices_buffer = &GLmock.buffers[ui->buffers.fill_vertices];
	GLmockBuffer *fill_indices_buffer = &GLmock.buffers[ui->buffers.fill_indices];
	GLmockBuffer *stroke_vertices_buffer = &GLmock.buffers[ui->buffers.stroke_vertices];

	_d(vertices_buffer->created);
	//=> 1
	_d(vertices_buffer->size);
	//=> 294912
	_d(vertices_buffer->data == ui->mesh.fill_vertices);
	//=> 1
	_d(vertices_buffer->usage == GL_STATIC_DRAW);
	//=> 1

	_d(fill_indices_buffer->created);
	//=> 1
	_d(fill_indices_buffer->size);
	//=> 98304
	_d(fill_indices_buffer->data == ui->mesh.fill_indices);
	//=> 1
	_d(fill_indices_buffer->usage == GL_STATIC_DRAW);
	//=> 1

	_d(stroke_vertices_buffer->created);
	//=> 1
	_d(stroke_vertices_buffer->size);
	//=> 294912
	_d(stroke_vertices_buffer->data == ui->mesh.stroke_vertices);
	//=> 1
	_d(stroke_vertices_buffer->usage == GL_STATIC_DRAW);
	//=> 1

	/////////////////////
	// textures

	_d(ui->textures.fill_colors);
	//=> 1
	_d(ui->textures.grid_styles);
	//=> 2

	GLmockTexture *fill_colors_texture = &GLmock.textures[ui->textures.fill_colors];
	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->textures.grid_styles];

	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_WRAP_S) == GL_CLAMP_TO_EDGE);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_WRAP_T) == GL_CLAMP_TO_EDGE);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_MAG_FILTER) == GL_NEAREST);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_MIN_FILTER) == GL_NEAREST);
	//=> 1

	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_WRAP_S) == GL_CLAMP_TO_EDGE);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_WRAP_T) == GL_CLAMP_TO_EDGE);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_MAG_FILTER) == GL_NEAREST);
	//=> 1
	_d(glmock_get_tex_parameter(ui->textures.fill_colors, GL_TEXTURE_MIN_FILTER) == GL_NEAREST);
	//=> 1

	_d(fill_colors_texture->created);
	//=> 1
	_dd(fill_colors_texture->width, fill_colors_texture->height);
	//=> 256, 1
	_d(fill_colors_texture->format == GL_RGBA);
	//=> 1
	_d(fill_colors_texture->type == GL_UNSIGNED_BYTE);
	//=> 1
	_d(fill_colors_texture->data == UI_FILL_COLORS);
	//=> 1

	_d(grid_styles_texture->created);
	//=> 1

	//////////////////////
	// styles_buffer

	_d(ui->styles_buffer != NULL);
	//=> 1
	_d(ui->styles_buffer_capacity);
	//=> 256
	_d(ui->styles_buffer_capacity_max);
	//=> 1024

	///////////////////////
	// view_matrix

	_gggg(ui->view_matrix.m[0][0], ui->view_matrix.m[1][0], ui->view_matrix.m[2][3], ui->view_matrix.m[3][3]);
	//=> 0, 0, 0, 0

	////////////////////////////
	// terminate

	ui_terminate(ui);

	_d(fill_vertex->deleted);
	//=> 1
	_d(stroke_vertex->deleted);
	//=> 1
	_d(fragment->deleted);
	//=> 1
	_d(fill_program->deleted);
	//=> 1
	_d(fill_program->deleted);
	//=> 1

	_d(vertices_buffer->deleted);
	//=> 1
	_d(fill_indices_buffer->deleted);
	//=> 1
	_d(stroke_vertices_buffer->deleted);
	//=> 1

	_d(fill_colors_texture->deleted);
	//=> 1
	_d(grid_styles_texture->deleted);
	//=> 1

	free(ui);
}

TEST(ui_initialize_pass_fail)
{
	Ui *ui = malloc(sizeof *ui);

	// Failures

	glmock_initialize();
	GLmock.shaders[1].compiled = -1;

	_d(ui_initialize(ui, 0));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error compiling shader. Nothing in info log.
	//=>

	glmock_initialize();
	GLmock.programs[1].linked = -1;

	_d(ui_initialize(ui, 0));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error linking program. Nothing in info log.
	//=>

	// Pass

	glmock_initialize();
	_d(ui_initialize(ui, 0));
	//=> 1
	_d(ui->styles_buffer_capacity_max);
	//=> 256

	ui_terminate(ui);
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
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &quad);
	ui_initialize(ui, 0);
	ui_update_styles(ui, g);

	Quad viewport_quad;
	view_viewport_to_quad(&vw, &viewport_quad);
	_qd(viewport_quad);
	//=> (-93, -27), (139, 54)

	ui_draw_fill(ui, &vw, g, &viewport_quad);

	_dd(GLmock.using_program, ui->fill_shader.program);
	//=> 1, 1

	_d(GLmock.bound_buffers[0] == ui->buffers.fill_vertices);
	//=> 1
	_d(GLmock.bound_buffers[1] == ui->buffers.fill_indices);
	//=> 1

	// Attributes

	GLmockProgram *program = &GLmock.programs[ui->fill_shader.program];

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

	GLmockUniform *grid_size = &program->uniforms[ui->fill_uniforms.gridSize];
	GLmockUniform *grid_position_offset = &program->uniforms[ui->fill_uniforms.gridPositionOffset];

	_gg(grid_size->fv0, grid_size->fv1);
	//=> 192, 128
	_hx(g->storage_quad.size);
	//=> 192, 128

	_gg(grid_position_offset->fv0, grid_position_offset->fv1);
	//=> 128, 0

	_d(program->uniforms[ui->fill_uniforms.viewMatrix].matrix4fv == &ui->view_matrix.m[0][0]);
	//=> 1

	// Textures

	_d(program->uniforms[ui->fill_uniforms.fillColors].iv0);
	//=> 0
	_d(program->uniforms[ui->fill_uniforms.gridStyles].iv0);
	//=> 1

	_d(GLmock.bound_textures[0] == ui->textures.fill_colors);
	//=> 1
	_d(GLmock.bound_textures[1] == ui->textures.grid_styles);
	//=> 1

	// Draw

	_d(GLmock.draw_elements_mode == GL_TRIANGLES);
	//=> 1
	_d(GLmock.draw_elements_type == GL_UNSIGNED_SHORT);
	//=> 1
	_d(GLmock.draw_elements_count);
	//=> 147456
	_d(49152 * 3);  // Draw the three showing blocks
	//=> 147456

	mat4 *m = &ui->view_matrix;
	_gggg(m->m[0][0], m->m[0][1], m->m[0][2], m->m[0][3]);
	//=> 0.001, 0, 0, 0
	_gggg(m->m[1][0], m->m[1][1], m->m[1][2], m->m[1][3]);
	//=> 0, 0.00166667, 0, 0
	_gggg(m->m[2][0], m->m[2][1], m->m[2][2], m->m[2][3]);
	//=> 0, 0, 0, 0
	_gggg(m->m[3][0], m->m[3][1], m->m[3][2], m->m[3][3]);
	//=> 0.0908512, 0.0333333, 0, 0.1

	ui_terminate(ui);
	grid_terminate(g);

	free(g);
	free(ui);
}

TEST(ui_draw_stroke)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {100, 100},
		.scale = 10.0,
	};
	Quad quad = {{-126, 1}, {253, 126}};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &quad);
	ui_initialize(ui, 0);
	ui_update_styles(ui, g);

	Quad viewport_quad;
	view_viewport_to_quad(&vw, &viewport_quad);
	_hx(viewport_quad.min);
	//=> -93, -27
	_hx(viewport_quad.max);
	//=> 139, 54

	ui_draw_stroke(ui, &vw, g, &viewport_quad);

	_dd(GLmock.using_program, ui->stroke_shader.program);
	//=> 2, 2

	_d(GLmock.bound_buffers[0] == ui->buffers.stroke_vertices);
	//=> 1

	// Attributes

	GLmockProgram *program = &GLmock.programs[ui->stroke_shader.program];

	GLmockAttribute *position = &program->attributes[ui->attributes.position];
	GLmockAttribute *gridPosition = &program->attributes[ui->attributes.gridPosition];
	GLmockAttribute *gridPosition2 = &program->attributes[ui->attributes.gridPosition2];

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

	_d(gridPosition2->size);
	//=> 2
	_d(gridPosition2->type == GL_BYTE);
	//=> 1
	_d(gridPosition2->stride);
	//=> 12
	_d(gridPosition2->offset);
	//=> 10
	_d(gridPosition2->enabled_vertex_attrib_array);
	//=> 1

	// Uniform

	GLmockUniform *grid_size = &program->uniforms[ui->stroke_uniforms.gridSize];
	GLmockUniform *grid_position_offset = &program->uniforms[ui->fill_uniforms.gridPositionOffset];
	GLmockUniform *stroke_color = &program->uniforms[ui->stroke_uniforms.strokeColor];

	_gg(grid_size->fv0, grid_size->fv1);
	//=> 192, 128
	_hx(g->storage_quad.size);
	//=> 192, 128

	_gg(grid_position_offset->fv0, grid_position_offset->fv1);
	//=> 128, 0

	_gggg(stroke_color->fv0, stroke_color->fv1, stroke_color->fv2, stroke_color->fv3);
	//=> 0.2, 0.2, 0.2, 1

	_d(program->uniforms[ui->stroke_uniforms.viewMatrix].matrix4fv == &ui->view_matrix.m[0][0]);
	//=> 1

	// Textures

	_d(program->uniforms[ui->stroke_uniforms.gridStyles].iv0);
	//=> 0

	_d(GLmock.bound_textures[0] == ui->textures.grid_styles);
	//=> 1

	// Draw

	_d(GLmock.draw_arrays_mode == GL_LINES);
	//=> 1
	_d(GLmock.draw_arrays_count);
	//=> 73728
	_d(24576 * 3);  // Draw the three showing blocks
	//=> 73728

	mat4 *m = &ui->view_matrix;
	_gggg(m->m[3][0], m->m[3][1], m->m[3][2], m->m[3][3]);
	//=> 0.0908512, 0.0333333, 0, 0.1

	ui_terminate(ui);
	grid_terminate(g);

	free(g);
	free(ui);
}

TEST(ui_update_styles)
{
	Quad quad = {{2, 1}, {125, 126}};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &quad);
	ui_initialize(ui, 0);

	ui_update_styles(ui, g);

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->textures.grid_styles];

	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 64, 128
	_d(grid_styles_texture->format == GL_ALPHA);
	//=> 1
	_d(grid_styles_texture->type == GL_UNSIGNED_BYTE);
	//=> 1
	_d(grid_styles_texture->data == g->styles);
	//=> 1

	grid_terminate(g);
	ui_terminate(ui);

	free(g);
	free(ui);
}

TEST(ui_update_styles_in_quad)
{
	Quad quad;
	StorageQuad sq;
	Quad grid_quad = {{2, 1}, {125, 62}};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	grid_initialize(g, &grid_quad);

	_d(ui_initialize(ui, 2500));
	//=> 1
	_d(ui->styles_buffer_capacity);
	//=> 256

	ui_update_styles(ui, g);

	// Under the current capacity
	quad = (Quad) {{10, 20}, {31, 59}};
	storage_quad_from_quad(&sq, &quad);
	_hx(hex_sub(sq.min, g->storage_quad.min));
	//=> 5, 20
	_hx(sq.size);
	//=> 11, 40
	
	ui_update_styles_in_quad(ui, g, &quad);

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->textures.grid_styles];

	_dd(grid_styles_texture->xoffset, grid_styles_texture->yoffset);
	//=> 5, 20
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 11, 40
	_d(grid_styles_texture->format == GL_ALPHA);
	//=> 1
	_d(grid_styles_texture->type == GL_UNSIGNED_BYTE);
	//=> 1
	_d(grid_styles_texture->data == ui->styles_buffer);
	//=> 1
	_d(ui->styles_buffer_capacity);
	//=> 440

	// Under/equal the max capacity
	quad = (Quad) {{10, 10}, {109, 59}};
	storage_quad_from_quad(&sq, &quad);
	_hx(sq.size);
	//=> 50, 50
	_d(storage_quad_capacity(&sq));
	//=> 2500

	ui_update_styles_in_quad(ui, g, &quad);

	_d(ui->styles_buffer_capacity);
	//=> 2500
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 50, 50

	// Over the max capacity
	quad = (Quad) {{10, 10}, {109, 60}};
	storage_quad_from_quad(&sq, &quad);
	_hx(hex_sub(sq.min, g->storage_quad.min));
	//=> 5, 10
	_hx(sq.size);
	//=> 50, 51
	_hx(g->storage_quad.size);
	//=> 64, 64

	ui_update_styles_in_quad(ui, g, &quad);

	_d(ui->styles_buffer_capacity);
	//=> 2500
	_dd(grid_styles_texture->width, grid_styles_texture->height);
	//=> 64, 64

	grid_terminate(g);
	ui_terminate(ui);

	free(g);
	free(ui);
}
