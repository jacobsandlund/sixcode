#include "test.h"
#include "bit-array.c"
#include "gl-mock.c"
#include "grid.c"
#include "hex.c"
#include "matrix.c"
#include "mesh.c"
#include "quad.c"
#include "ui.c"
#include "view.c"

#define _hx(h) _dd(h.c, h.r)

TEST(ui)
{
	Ui *ui = malloc(sizeof *ui);

	gl_mock_initialize();

	_d(ui_initialize(ui, 1024));
	//=> 1

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
	//=> 3, 1

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
	_dd(GL_MOCK.texture_width[0], GL_MOCK.texture_height[0]);
	//=> 256, 1
	_d(GL_MOCK.texture_data[0] == UI_FILL_COLORS);
	//=> 1

	//////////////////////
	// styles_buffer

	_d(ui->styles_buffer != NULL);
	//=> 1
	_d(ui->styles_buffer_capacity);
	//=> 256
	_d(ui->styles_buffer_capacity_max);
	//=> 1024

	////////////////////////////
	// terminate

	ui_terminate(ui);

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
}

TEST(ui_initialize_pass_fail)
{
	Ui *ui = malloc(sizeof *ui);

	// Failures

	gl_mock_initialize();
	GL_MOCK.shader_compile_status[1] = -1;

	_d(ui_initialize(ui, 0));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error compiling shader. Nothing in info log.
	//=>

	gl_mock_initialize();
	GL_MOCK.force_create_program_fail = 1;
	GL_MOCK.force_gl_error = GL_INVALID_OPERATION;

	_d(ui_initialize(ui, 0));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> ./ui.c:138 - The specified operation is not allowed in the current state.
	//=> Error creating program.
	//=>

	gl_mock_initialize();
	GL_MOCK.program_link_status[1] = -1;

	_d(ui_initialize(ui, 0));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error linking program. Nothing in info log.
	//=>

	// Pass

	gl_mock_initialize();
	_d(ui_initialize(ui, 0));
	//=> 1
	_d(ui->styles_buffer_capacity_max);
	//=> 256

	ui_terminate(ui);
	free(ui);
}

TEST(ui_draw)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 250};
	f32 scale = 10.0;
	Quad quad = {{0, 0}, {127, 63}};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);
	View *vw = malloc(sizeof *vw);

	gl_mock_initialize();
	grid_initialize(g, &quad);
	view_initialize(vw, viewport_size, translation, scale);
	ui_initialize(ui, 0);
	ui_update_styles(ui, g);

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
	//=> 49152

	ui_terminate(ui);
	grid_terminate(g);

	free(g);
	free(ui);
	free(vw);
}

TEST(ui_update_styles)
{
	Quad quad = {{0, 0}, {127, 127}};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	gl_mock_initialize();
	grid_initialize(g, &quad);
	ui_initialize(ui, 0);

	ui_update_styles(ui, g);

	_dd(GL_MOCK.texture_width[1], GL_MOCK.texture_height[1]);
	//=> 64, 128
	_d(GL_MOCK.texture_data[1] == g->styles);
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
	Quad grid_quad = {{40, 10}, {167, 73}};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	gl_mock_initialize();
	grid_initialize(g, &grid_quad);

	_d(ui_initialize(ui, 2500));
	//=> 1
	_d(ui->styles_buffer_capacity);
	//=> 256

	ui_update_styles(ui, g);

	quad = (Quad) {{60, 30}, {71, 69}};
	storage_quad_from_quad(&sq, &quad);
	_hx(hex_sub(sq.min, g->storage_quad.min));
	//=> 10, 20
	_hx(sq.size);
	//=> 6, 40
	
	// Under the current capacity
	ui_update_styles_in_quad(ui, g, &quad);

	_dd(GL_MOCK.sub_texture_xoffset[1], GL_MOCK.sub_texture_yoffset[1]);
	//=> 10, 20
	_dd(GL_MOCK.texture_width[1], GL_MOCK.texture_height[1]);
	//=> 6, 40
	_d(ui->styles_buffer_capacity);
	//=> 256
	_d(GL_MOCK.texture_data[1] == ui->styles_buffer);
	//=> 1

	quad = (Quad) {{50, 20}, {149, 69}};
	storage_quad_from_quad(&sq, &quad);
	_hx(sq.size);
	//=> 50, 50
	_d(storage_quad_capacity(&sq));
	//=> 2500

	// Under/equal the max capacity
	ui_update_styles_in_quad(ui, g, &quad);

	_d(ui->styles_buffer_capacity);
	//=> 2500
	_dd(GL_MOCK.texture_width[1], GL_MOCK.texture_height[1]);
	//=> 50, 50

	quad = (Quad) {{50, 20}, {149, 70}};
	storage_quad_from_quad(&sq, &quad);
	_hx(hex_sub(sq.min, g->storage_quad.min));
	//=> 5, 10
	_hx(sq.size);
	//=> 50, 51
	_hx(g->storage_quad.size);
	//=> 64, 64

	// Over the max capacity
	ui_update_styles_in_quad(ui, g, &quad);

	_d(ui->styles_buffer_capacity);
	//=> 2500
	_dd(GL_MOCK.texture_width[1], GL_MOCK.texture_height[1]);
	//=> 64, 64

	grid_terminate(g);
	ui_terminate(ui);

	free(g);
	free(ui);
}
