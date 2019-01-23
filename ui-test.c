#include "test.h"
#include "bit-array.c"
#include "gl-mock.c"
#include "grid.c"
#include "matrix.c"
#include "mesh.c"
#include "quad.c"
#include "ui.c"
#include "view.c"

TEST(ui)
{
	Quad quad = {
		{0, 0},
		{63, 63},
	};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	gl_mock_initialize();
	grid_initialize(g, quad);

	ui_initialize(ui, g);

	///////////////////////
	// load/create + attach

	_dd(ui->vertex_shader, GL_MOCK.shaders[ui->vertex_shader]);
	//=> 1, 1
	_dd(ui->fragment_shader, GL_MOCK.shaders[ui->fragment_shader]);
	//=> 2, 1
	_dd(ui->program, GL_MOCK.programs[ui->program]);
	//=> 1, 1

	_d(GL_MOCK.shader_attachments[ui->vertex_shader]);
	//=> 1
	_d(GL_MOCK.shader_attachments[ui->fragment_shader]);
	//=> 1

	/////////////////////
	// locations

	_d(ui->locations.position);
	//=> 0
	_d(ui->locations.gridPosition);
	//=> 1
	_d(ui->locations.viewMatrix);
	//=> 1
	_d(ui->locations.fillColors);
	//=> 2
	_d(ui->locations.gridStyles);
	//=> 3

	/////////////////////
	// compiled + linked

	GLint compiled;
	glGetShaderiv(ui->vertex_shader, GL_COMPILE_STATUS, &compiled);
	_d(compiled);
	//=> 1
	glGetShaderiv(ui->fragment_shader, GL_COMPILE_STATUS, &compiled);
	_d(compiled);
	//=> 1

	GLint linked;
	glGetProgramiv(ui->program, GL_LINK_STATUS, &linked);
	_d(linked);
	//=> 1

	/////////////////////
	// mesh + buffers

	_d(ui->hex_mesh.vertices_length);
	//=> 24576
	_d(ui->hex_mesh.fill_indices_length);
	//=> 49152

	_dd(ui->buffers.hex_mesh_vertices, GL_MOCK.buffers[ui->buffers.hex_mesh_vertices]);
	//=> 1, 1
	_dd(ui->buffers.hex_mesh_fill_indices, GL_MOCK.buffers[ui->buffers.hex_mesh_fill_indices]);
	//=> 2, 1
	_dd(ui->buffers.hex_mesh_stroke_indices, GL_MOCK.buffers[ui->buffers.hex_mesh_stroke_indices]);
	//=> 0, 0

	_d(GL_MOCK.bound_buffers[0] == ui->buffers.hex_mesh_vertices);
	//=> 1
	_d(GL_MOCK.bound_buffers[1] == ui->buffers.hex_mesh_fill_indices);
	//=> 1
	_d(GL_MOCK.buffer_data[0] == ui->hex_mesh.vertices);
	//=> 1
	_d(GL_MOCK.buffer_data[1] == ui->hex_mesh.fill_indices);
	//=> 1

	/////////////////////
	// textures

	_dd(ui->textures.fill_colors, GL_MOCK.textures[ui->textures.fill_colors]);
	//=> 1, 1
	_dd(ui->textures.grid_styles, GL_MOCK.textures[ui->textures.grid_styles]);
	//=> 2, 1
	_d(GL_MOCK.texture_data[0] == UI_FILL_COLORS);
	//=> 1
	_d(GL_MOCK.texture_data[1] == g->styles);
	//=> 1
	_dd(GL_MOCK.texture_width[0], GL_MOCK.texture_height[0]);
	//=> 16, 1
	_dd(GL_MOCK.texture_width[1], GL_MOCK.texture_height[1]);
	//=> 64, 64

	////////////////////////////
	// terminate

	ui_terminate(ui);
	grid_terminate(g);

	_d(GL_MOCK.shaders[ui->vertex_shader]);
	//=> 0
	_d(GL_MOCK.shaders[ui->fragment_shader]);
	//=> 0
	_d(GL_MOCK.programs[ui->program]);
	//=> 0

	_d(GL_MOCK.shader_attachments[ui->vertex_shader]);
	//=> 0
	_d(GL_MOCK.shader_attachments[ui->fragment_shader]);
	//=> 0

	_d(GL_MOCK.buffers[ui->buffers.hex_mesh_vertices]);
	//=> 0
	_d(GL_MOCK.buffers[ui->buffers.hex_mesh_fill_indices]);
	//=> 0
	_d(GL_MOCK.buffers[ui->buffers.hex_mesh_stroke_indices]);
	//=> 0

	_d(GL_MOCK.textures[ui->textures.fill_colors]);
	//=> 0
	_d(GL_MOCK.textures[ui->textures.grid_styles]);
	//=> 0

	free(ui);
	free(g);
}

TEST(ui_draw)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 250};
	f32 scale = 10.0;
	Quad quad = {
		{0, 0},
		{63, 63},
	};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);
	View *vw = malloc(sizeof *vw);

	gl_mock_initialize();
	grid_initialize(g, quad);
	view_initialize(vw, viewport_size, translation, scale);

	ui_initialize(ui, g);

	view_update_matrix(vw);

	ui_draw(ui, vw);

	_d(GL_MOCK.viewport_width);
	//=> 1000
	_d(GL_MOCK.viewport_height);
	//=> 600

	_d(GL_MOCK.attib_pointer_stride);
	//=> 12

	_d(GL_MOCK.uniform_matrix4_value == &vw->view_matrix.m[0][0]);
	//=> 1

	_d(GL_MOCK.draw_elements_count);
	//=> 12

	ui_terminate(ui);
	grid_terminate(g);

	free(g);
	free(ui);
	free(vw);
}
