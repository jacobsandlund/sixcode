#include "ui.c"
#include "test.h"
#include "glmock.c"
#include "grid.c"
#include "mesh.c"
#include "quad.c"
#include "shader.c"
#include "texture.c"
#include "view.c"

TEST(ui)
{
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();

	_d(ui_initialize(ui));
	//=> 1

	_d(ui->blend_enabled);
	//=> 1

	///////////////////////
	// load/create/link

	_d(ui->shader.vertex);
	//=> 1
	_d(ui->shader.fragment);
	//=> 2
	
	GLmockShader *vertex = &GLmock.shaders[ui->shader.vertex];
	GLmockShader *fragment = &GLmock.shaders[ui->shader.fragment];
	_dd(vertex->created, vertex->compiled);
	//=> 1, 1
	_dd(fragment->created, fragment->compiled);
	//=> 1, 1

	_d(ui->shader.program);
	//=> 1
	
	GLmockProgram *program = &GLmock.programs[ui->shader.program];

	_d(program->created);
	//=> 1
	_d(program->attached_vertex_shader);
	//=> 1
	_d(program->attached_fragment_shader);
	//=> 2

	_d(program->linked);
	//=> 1

	/////////////////////
	// attributes

	_d(ui->attributes.position);
	//=> 1
	_d(ui->attributes.gridPosition);
	//=> 2
	_d(ui->attributes.positionOffset);
	//=> 3
	_d(ui->attributes.gridPositionOffset);
	//=> 4

	_s(program->attributes[ui->attributes.position].name);
	//=> position
	_s(program->attributes[ui->attributes.gridPosition].name);
	//=> gridPosition
	_s(program->attributes[ui->attributes.positionOffset].name);
	//=> positionOffset
	_s(program->attributes[ui->attributes.gridPositionOffset].name);
	//=> gridPositionOffset

	////////////////////
	// uniforms

	_d(ui->uniforms.viewMatrix);
	//=> 1
	_d(ui->uniforms.gridSize);
	//=> 2
	_d(ui->uniforms.styleOffset);
	//=> 3
	_d(ui->uniforms.gridStyles);
	//=> 4
	_d(ui->uniforms.fillColors);
	//=> 5

	_s(program->uniforms[ui->uniforms.viewMatrix].name);
	//=> viewMatrix
	_s(program->uniforms[ui->uniforms.gridSize].name);
	//=> gridSize
	_s(program->uniforms[ui->uniforms.styleOffset].name);
	//=> styleOffset
	_s(program->uniforms[ui->uniforms.gridStyles].name);
	//=> gridStyles
	_s(program->uniforms[ui->uniforms.fillColors].name);
	//=> fillColors

	/////////////////////
	// textures

	_d(ui->grid_styles_texture);
	//=> 1
	_d(ui->fill_colors_texture);
	//=> 2

	GLmockTexture *grid_styles_texture = &GLmock.textures[ui->grid_styles_texture.texture];
	GLmockTexture *fill_colors_texture = &GLmock.textures[ui->fill_colors_texture.texture];

	_d(grid_styles_texture->created);
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

	/////////////////////
	// mesh + buffers

	_d(ui->layouts[0].meshes[0].vertices_length);
	//=> 384
	_d(ui->layouts[0].meshes[1].vertices_length);
	//=> 96
	_d(ui->layouts[0].meshes[2].vertices_length);
	//=> 24
	_d(ui->layouts[0].meshes[3].vertices_length);
	//=> 6
	_d(ui->layouts[0].meshes[0].indices_length);
	//=> 768
	_d(ui->layouts[0].meshes[3].indices_length);
	//=> 12

	_d(ui->layouts[1].meshes[0].vertices_length);
	//=> 256
	_d(ui->layouts[1].meshes[3].vertices_length);
	//=> 4
	_d(ui->layouts[1].meshes[0].indices_length);
	//=> 384
	_d(ui->layouts[1].meshes[3].indices_length);
	//=> 6

	_d(ui->layouts[0].buffers[0].vertices);
	//=> 1
	_d(ui->layouts[0].buffers[1].vertices);
	//=> 3
	_d(ui->layouts[0].buffers[2].vertices);
	//=> 5
	_d(ui->layouts[0].buffers[3].vertices);
	//=> 7
	_d(ui->layouts[0].buffers[0].indices);
	//=> 2
	_d(ui->layouts[0].buffers[3].indices);
	//=> 8

	_d(ui->layouts[1].buffers[0].vertices);
	//=> 9
	_d(ui->layouts[1].buffers[3].vertices);
	//=> 15
	_d(ui->layouts[1].buffers[0].indices);
	//=> 10
	_d(ui->layouts[1].buffers[3].indices);
	//=> 16

	_d(ui->instanceBuffer);
	//=> 17
	_d(ui->instance_mesh.vertices_length);
	//=> 32

	GLmockBuffer *vertices_buffer = &GLmock.buffers[ui->layouts[0].buffers[0].vertices];
	GLmockBuffer *indices_buffer = &GLmock.buffers[ui->layouts[1].buffers[1].indices];
	GLmockBuffer *instance_buffer = &GLmock.buffers[ui->instanceBuffer];

	_d(vertices_buffer->created);
	//=> 1
	_d(vertices_buffer->size);
	//=> 4608
	_d(vertices_buffer->data == ui->layouts[0].meshes[0].vertices);
	//=> 1
	_d(vertices_buffer->usage == GL_STATIC_DRAW);
	//=> 1

	_d(indices_buffer->created);
	//=> 1
	_d(indices_buffer->size);
	//=> 192
	_d(indices_buffer->data == ui->layouts[1].meshes[1].indices);
	//=> 1
	_d(indices_buffer->usage == GL_STATIC_DRAW);
	//=> 1

	_d(instance_buffer->created);
	//=> 1

	////////////////////////////
	// terminate

	ui_terminate(ui);

	_d(vertex->deleted);
	//=> 1
	_d(program->deleted);
	//=> 1

	_d(vertices_buffer->deleted);
	//=> 1
	_d(indices_buffer->deleted);
	//=> 1
	_d(instance_buffer->deleted);
	//=> 1

	_d(grid_styles_texture->deleted);
	//=> 1
	_d(fill_colors_texture->deleted);
	//=> 1

	free(ui);
}

TEST(ui_initialize_fail)
{
	Ui *ui = malloc(sizeof *ui);

	// Failures

	glmock_initialize();
	GLmock.shaders[1].force_compile_error = true;

	_d(ui_initialize(ui));
	//=> 0
        _TEST_SPACETIME_ERROR();
        //=> Error compiling shader. Nothing in info log.
	//=>

	glmock_initialize();
	GLmock.programs[1].force_link_error = true;

	_d(ui_initialize(ui));
	//=> 0
        _TEST_SPACETIME_ERROR();
        //=> Error linking program. Nothing in info log.
	//=>

	free(ui);
}

TEST(ui_draw_fill)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 100};
	float scale = 10.0;

	View *vw = malloc(sizeof *vw);
	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	view_initialize(vw, viewport_size, translation, scale);
	grid_initialize(g);
	ui_initialize(ui);
	texture_update(&ui->grid_styles_texture, g);

	ui_draw(ui, vw, g);

	_dd(GLmock.using_program, ui->shader.program);
	//=> 1, 1

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

	_gg(grid_size->fv0, grid_size->fv1);
	//=> 4096, 4096
	_i2(g->size_quad.size);
	//=> 4096, 4096

	_d(program->uniforms[ui->uniforms.styleOffset].iv0);
	//=> 0

	mat4 *m = &vw->view_matrix;
	_d(program->uniforms[ui->uniforms.viewMatrix].matrix4fv == &m->m[0][0]);
	//=> 1

	_gg(m->m[0][0], m->m[1][1]);
	//=> 0.00173205, -0.0025
	_ggg(m->m[3][0], m->m[3][1], m->m[3][3]);
	//=> -0.0502295, 0.05, 0.05

	// Textures

	_d(program->uniforms[ui->uniforms.gridStyles].iv0);
	//=> 0
	_d(program->uniforms[ui->uniforms.fillColors].iv0);
	//=> 1

	_d(GLmock.bound_textures[0] == ui->grid_styles_texture.texture);
	//=> 1
	_d(GLmock.bound_textures[1] == ui->fill_colors_texture.texture);
	//=> 1

	// Blend + Fill color

	_d(GLmock.enabled_capability == GL_BLEND);
	//=> 1
	_d(GLmock.blend_source_factor == GL_SRC_ALPHA);
	//=> 1
	_d(GLmock.blend_destination_factor == GL_ZERO);
	//=> 1

	GLmockTexture *fill_colors_texture = &GLmock.textures[ui->fill_colors_texture.texture];
	_d(fill_colors_texture->data == UI_FILL_COLORS);
	//=> 1
	_dd(fill_colors_texture->width, fill_colors_texture->height);
	//=> 1, 1
	_dd(fill_colors_texture->xoffset, fill_colors_texture->yoffset);
	//=> 0, 0

	// Instance Buffer

	InstanceMesh *imesh = &ui->instance_mesh;
	_d(imesh->vertices_length);
	//=> 48

	_v2(imesh->vertices[0].positionOffset);
	//=> 0, 0
	_i2(imesh->vertices[0].gridPositionOffset);
	//=> 2119, 2128

	_v2(imesh->vertices[imesh->vertices_length - 1].positionOffset);
	//=> 56, 40
	_i2(imesh->vertices[imesh->vertices_length - 1].gridPositionOffset);
	//=> 2175, 2168

	GLmockBuffer *instance_buffer = &GLmock.buffers[ui->instanceBuffer];
	_d(instance_buffer->size);
	//=> 768
	_d(instance_buffer->data == ui->instance_mesh.vertices);
	//=> 1
	_d(instance_buffer->usage == GL_STREAM_DRAW);
	//=> 1

	// Instance attributes

	GLmockAttribute *positionOffset = &program->attributes[ui->attributes.positionOffset];
	GLmockAttribute *gridPositionOffset = &program->attributes[ui->attributes.gridPositionOffset];

	_d(positionOffset->size);
	//=> 2
	_d(positionOffset->type == GL_FLOAT);
	//=> 1
	_d(positionOffset->stride);
	//=> 16
	_d(positionOffset->offset);
	//=> 0
	_d(positionOffset->divisor);
	//=> 1
	_d(positionOffset->enabled_vertex_attrib_array);
	//=> 1

	_d(gridPositionOffset->size);
	//=> 2
	_d(gridPositionOffset->type == GL_INT);
	//=> 1
	_d(gridPositionOffset->stride);
	//=> 16
	_d(gridPositionOffset->offset);
	//=> 8
	_d(gridPositionOffset->divisor);
	//=> 1
	_d(gridPositionOffset->enabled_vertex_attrib_array);
	//=> 1

	// Draw

	_d(GLmock.bound_buffers[0] == ui->instanceBuffer);
	//=> 1
	_d(GLmock.bound_buffers[1] == ui->layouts[0].buffers[0].indices);
	//=> 1

	_d(GLmock.draw_elements_mode == GL_TRIANGLES);
	//=> 1
	_d(GLmock.draw_elements_type == GL_UNSIGNED_SHORT);
	//=> 1
	_d(GLmock.draw_elements_count);
	//=> 36864
	_d(GLmock.draw_elements_instanced_primcount);
	//=> 48

	grid_terminate(g);
	ui_terminate(ui);

	free(vw);
	free(g);
	free(ui);
}

TEST(ui_draw_fill_blend_or_not)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 100};
	float scale = 6.0;

	View *vw = malloc(sizeof *vw);
	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	view_initialize(vw, viewport_size, translation, scale);
	grid_initialize(g);
	ui_initialize(ui);
	texture_update(&ui->grid_styles_texture, g);

	ui_draw(ui, vw, g);

	// Zoomed out == no blend

	_d(GLmock.disabled_capability == GL_BLEND);
	//=> 1

	GLmockTexture *fill_colors_texture = &GLmock.textures[ui->fill_colors_texture.texture];
	_d(fill_colors_texture->data == UI_EMPTY_FILL_COLOR_NO_BLEND);
	//=> 1

	// Blend disabled

	ui_terminate(ui);

	scale = 10.0;
	view_initialize(vw, viewport_size, translation, scale);
	glmock_initialize();
	ui_initialize(ui);
	texture_update(&ui->grid_styles_texture, g);
	ui->blend_enabled = false;

	ui_draw(ui, vw, g);

	_d(GLmock.disabled_capability == GL_BLEND);
	//=> 1

	_d(fill_colors_texture->data == UI_EMPTY_FILL_COLOR_NO_BLEND);
	//=> 1

	grid_terminate(g);
	ui_terminate(ui);

	free(vw);
	free(g);
	free(ui);
}

TEST(ui_draw_fill_rect)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 100};
	float scale = 10.0;

	View *vw = malloc(sizeof *vw);
	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	glmock_initialize();
	view_initialize(vw, viewport_size, translation, scale);
	view_layout(vw, VIEW_LAYOUT_RECT);
	grid_initialize(g);
	ui_initialize(ui);
	texture_update(&ui->grid_styles_texture, g);

	ui_draw(ui, vw, g);

	// Draw

	_d(GLmock.bound_buffers[0] == ui->instanceBuffer);
	//=> 1
	_d(GLmock.bound_buffers[1] == ui->layouts[1].buffers[0].indices);
	//=> 1

	_d(GLmock.draw_elements_mode == GL_TRIANGLES);
	//=> 1
	_d(GLmock.draw_elements_type == GL_UNSIGNED_SHORT);
	//=> 1
	_d(GLmock.draw_elements_count);
	//=> 18432
	_d(GLmock.draw_elements_instanced_primcount);
	//=> 48

	grid_terminate(g);
	ui_terminate(ui);

	free(vw);
	free(g);
	free(ui);
}
