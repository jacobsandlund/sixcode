#include "test.h"
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
	_d(ui->locations.gridSize);
	//=> 2
	_d(ui->locations.gridPositionOffset);
	//=> 3
	_d(ui->locations.fillColors);
	//=> 4
	_d(ui->locations.gridStyles);
	//=> 5

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

	///////////////////////
	// view_matrix

	_gggg(ui->view_matrix.m[0][0], ui->view_matrix.m[1][0], ui->view_matrix.m[2][3], ui->view_matrix.m[3][3]);
	//=> 0, 0, 0, 0

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
	//=> ./ui.c:139 - The specified operation is not allowed in the current state.
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
	vec2 translation = {100, 100};
	f32 scale = 10.0;
	Quad quad = {{-128, 0}, {255, 127}};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);
	View *vw = malloc(sizeof *vw);

	gl_mock_initialize();
	grid_initialize(g, &quad);
	view_initialize(vw, viewport_size, translation, scale);
	ui_initialize(ui, 0);
	ui_update_styles(ui, g);

	view_viewport_to_quad(vw, &quad);
	_hx(quad.min);
	//=> -93, -27
	_hx(quad.max);
	//=> 139, 54

	ui_draw(ui, vw, g);

	// Global state

	_d(GL_MOCK.viewport_width);
	//=> 1000
	_d(GL_MOCK.viewport_height);
	//=> 600

	// Attributes

	_d(GL_MOCK.bound_buffers[0] == ui->buffers.hex_mesh_vertices);
	//=> 1
	_d(GL_MOCK.bound_buffers[1] == ui->buffers.hex_mesh_fill_indices);
	//=> 1
	_d(GL_MOCK.attrib_pointer_offset);
	//=> 8
	_d(GL_MOCK.attrib_pointer_stride);
	//=> 12

	// Uniforms

	_d(GL_MOCK.uniform_matrix4_value == &ui->view_matrix.m[0][0]);
	//=> 1

	// Textures

	_d(GL_MOCK.bound_textures[0] == ui->textures.fill_colors);
	//=> 1
	_d(GL_MOCK.bound_textures[1] == ui->textures.grid_styles);
	//=> 1

	// Draw

	_d(GL_MOCK.draw_elements_count);
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
	Quad grid_quad = {{0, 0}, {127, 63}};

	Grid *g = malloc(sizeof *g);
	Ui *ui = malloc(sizeof *ui);

	gl_mock_initialize();
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

	_dd(GL_MOCK.sub_texture_xoffset[1], GL_MOCK.sub_texture_yoffset[1]);
	//=> 5, 20
	_dd(GL_MOCK.texture_width[1], GL_MOCK.texture_height[1]);
	//=> 11, 40
	_d(ui->styles_buffer_capacity);
	//=> 440
	_d(GL_MOCK.texture_data[1] == ui->styles_buffer);
	//=> 1

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
	_dd(GL_MOCK.texture_width[1], GL_MOCK.texture_height[1]);
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
	_dd(GL_MOCK.texture_width[1], GL_MOCK.texture_height[1]);
	//=> 64, 64

	grid_terminate(g);
	ui_terminate(ui);

	free(g);
	free(ui);
}
