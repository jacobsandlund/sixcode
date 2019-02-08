#include <stdio.h>
#include <string.h>
#include "ui.h"

#define UI_FILL_COLORS_COUNT 256
#define UI_FILL_COLOR_COMPONENTS_LENGTH 1024  // 256 * 4
#define UI_STYLES_BUFFER_CAPACITY_MIN 256

// 0.001953125 = 0.5 / 256
const char UI_FILL_VERTEX_SHADER_SOURCE[] =
"attribute vec4 position;\n"
"attribute vec2 gridPosition;\n"
"\n"
"uniform mat4 viewMatrix;\n"
"uniform vec2 gridSize;\n"
"uniform vec2 gridPositionOffset;\n"
"\n"
"uniform sampler2D fillColors;\n"
"uniform sampler2D gridStyles;\n"
"\n"
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	vec2 styleCoord = (gridPosition + gridPositionOffset) / gridSize;\n"
"	float style = texture2D(gridStyles, styleCoord).a + 0.001953125;\n"
"	color = texture2D(fillColors, vec2(style, 0.5));\n"
"	gl_Position = viewMatrix * position;\n"
"}\n";

const char UI_STROKE_VERTEX_SHADER_SOURCE[] =
"attribute vec4 position;\n"
"attribute vec2 gridPosition;\n"
"\n"
"uniform mat4 viewMatrix;\n"
"uniform vec2 gridSize;\n"
"uniform vec2 gridPositionOffset;\n"
"uniform vec4 strokeColor;\n"
"\n"
"uniform sampler2D gridStyles;\n"
"\n"
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	vec2 styleCoord = (gridPosition + gridPositionOffset) / gridSize;\n"
"	float present = ceil(texture2D(gridStyles, styleCoord).a);\n"
"	color = strokeColor * present;\n"
"	gl_Position = viewMatrix * position;\n"
"}\n";

const char UI_FRAGMENT_SHADER_SOURCE[] =
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	gl_FragColor = color;\n"
"}\n";

const u8 UI_FILL_COLORS[UI_FILL_COLOR_COMPONENTS_LENGTH] = {
      0,   0,   0,   0,  // Not present
    190, 190, 190, 255,
    255, 140, 140, 255,
    140, 255, 140, 255,

    140, 140, 255, 255,
    255, 255,  40, 255,
    255,  40, 255, 255,
     40, 255, 255, 255,

    255, 190,  90, 255,
    255,  90, 190, 255,
    190, 255,  90, 255,
     90, 255, 190, 255,

    190,  90, 255, 255,
     90, 190, 255, 255,
    220, 190, 140, 255,
    140, 190, 220, 255,
};

i8 ui_initialize(Ui *ui, i32 styles_buffer_capacity_max)
{
	////////////////////////
	// load/create

	ui->fill_shader.vertex = shader_load(GL_VERTEX_SHADER, UI_FILL_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	ui->fill_shader.fragment = shader_load(GL_FRAGMENT_SHADER, UI_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);
	ui->stroke_shader.vertex = shader_load(GL_VERTEX_SHADER, UI_STROKE_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	ui->stroke_shader.fragment = ui->fill_shader.fragment;

	if (
		!shader_program_create(&ui->fill_shader, __FILE__, __LINE__) ||
		!shader_program_create(&ui->stroke_shader, __FILE__, __LINE__)
	) {
		shader_program_delete_shaders(&ui->fill_shader);
		shader_program_delete_shaders(&ui->stroke_shader);

		return 0;
	}

	////////////////////
	// attributes

	ui->attributes.position     = 0;
	ui->attributes.gridPosition = 1;

	glBindAttribLocation(
			ui->fill_shader.program,
			ui->attributes.position,
			"position");

	glBindAttribLocation(
			ui->fill_shader.program,
			ui->attributes.gridPosition,
			"gridPosition");

	glBindAttribLocation(
			ui->stroke_shader.program,
			ui->attributes.position,
			"position");

	glBindAttribLocation(
			ui->stroke_shader.program,
			ui->attributes.gridPosition,
			"gridPosition");

	////////////////////
	// link

	if (
		!shader_program_link(&ui->fill_shader, __FILE__, __LINE__) ||
		!shader_program_link(&ui->stroke_shader, __FILE__, __LINE__)
	) {
		shader_program_delete_shaders(&ui->fill_shader);
		shader_program_delete_shaders(&ui->stroke_shader);

		return 0;
	}

	////////////////////
	// uniforms

	ui->fill_uniforms.viewMatrix = glGetUniformLocation(
			ui->fill_shader.program,
			"viewMatrix");

	ui->fill_uniforms.gridSize = glGetUniformLocation(
			ui->fill_shader.program,
			"gridSize");

	ui->fill_uniforms.gridPositionOffset = glGetUniformLocation(
			ui->fill_shader.program,
			"gridPositionOffset");

	ui->fill_uniforms.fillColors = glGetUniformLocation(
			ui->fill_shader.program,
			"fillColors");

	ui->fill_uniforms.gridStyles = glGetUniformLocation(
			ui->fill_shader.program,
			"gridStyles");

	ui->stroke_uniforms.viewMatrix = glGetUniformLocation(
			ui->stroke_shader.program,
			"viewMatrix");

	ui->stroke_uniforms.gridSize = glGetUniformLocation(
			ui->stroke_shader.program,
			"gridSize");

	ui->stroke_uniforms.gridPositionOffset = glGetUniformLocation(
			ui->stroke_shader.program,
			"gridPositionOffset");

	ui->stroke_uniforms.strokeColor = glGetUniformLocation(
			ui->stroke_shader.program,
			"strokeColor");

	ui->stroke_uniforms.gridStyles = glGetUniformLocation(
			ui->stroke_shader.program,
			"gridStyles");

	//////////////////
	// mesh + buffers

	mesh_initialize(&ui->mesh, UI_MESH_SIZE, UI_MESH_SIZE);

	glGenBuffers(1, &ui->buffers.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, ui->buffers.vertices);
	glBufferData(
			GL_ARRAY_BUFFER,
			ui->mesh.vertices_length * sizeof *ui->mesh.vertices,
			ui->mesh.vertices,
			GL_STATIC_DRAW);

	glGenBuffers(1, &ui->buffers.fill_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui->buffers.fill_indices);
	glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			ui->mesh.fill_indices_length * sizeof *ui->mesh.fill_indices,
			ui->mesh.fill_indices,
			GL_STATIC_DRAW);

	glGenBuffers(1, &ui->buffers.stroke_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui->buffers.stroke_indices);
	glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			ui->mesh.stroke_indices_length * sizeof *ui->mesh.stroke_indices,
			ui->mesh.stroke_indices,
			GL_STATIC_DRAW);
	
	////////////////
	// textures

	glGenTextures(1, &ui->textures.fill_colors);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui->textures.fill_colors);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			UI_FILL_COLORS_COUNT,
			1,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			UI_FILL_COLORS);

	glGenTextures(1, &ui->textures.grid_styles);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ui->textures.grid_styles);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	///////////////////////
	// styles_buffer

	if (styles_buffer_capacity_max < UI_STYLES_BUFFER_CAPACITY_MIN) {
		styles_buffer_capacity_max = UI_STYLES_BUFFER_CAPACITY_MIN;
	}

	ui->styles_buffer = malloc(UI_STYLES_BUFFER_CAPACITY_MIN * sizeof *ui->styles_buffer);
	ui->styles_buffer_capacity = UI_STYLES_BUFFER_CAPACITY_MIN;
	ui->styles_buffer_capacity_max = styles_buffer_capacity_max;

	///////////////////////
	// view_matrix

	for (i32 i = 0; i < 4; i++) {
		for (i32 j = 0; j < 4; j++) {
			ui->view_matrix.m[i][j] = 0.0f;
		}
	}

	return 1;
}

void ui_terminate(Ui *ui)
{
	shader_program_delete(&ui->fill_shader);
	shader_program_delete(&ui->stroke_shader);

	shader_program_delete_shaders(&ui->fill_shader);
	shader_program_delete_shaders(&ui->stroke_shader);

	mesh_terminate(&ui->mesh);

	glDeleteBuffers(1, &ui->buffers.vertices);
	glDeleteBuffers(1, &ui->buffers.fill_indices);
	glDeleteBuffers(1, &ui->buffers.stroke_indices);

	glDeleteTextures(1, &ui->textures.fill_colors);
	glDeleteTextures(1, &ui->textures.grid_styles);

	free(ui->styles_buffer);
}

static void ui_configure_attributes(Ui *ui)
{
	glVertexAttribPointer(
			ui->attributes.position,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof ui->mesh.vertices[0],
			0);
	glEnableVertexAttribArray(ui->attributes.position);

	glVertexAttrib4f(ui->attributes.position, 0.0f, 0.0f, 0.0f, 1.0f);

	glVertexAttribPointer(
			ui->attributes.gridPosition,
			2,
			GL_BYTE,
			GL_FALSE,
			sizeof ui->mesh.vertices[0],
			(GLvoid *) (2 * sizeof ui->mesh.vertices[0].x));
	glEnableVertexAttribArray(ui->attributes.gridPosition);
}

static void ui_update_view_matrix(Ui *ui, View *vw)
{
	f64 scale_inv = 1.0 / vw->scale;

	ui->translation_x = -vw->translation.x * scale_inv * 2.0;
	ui->translation_y = vw->translation.y * scale_inv * 2.0;

	ui->view_matrix.m[0][0] = 1.0 / (f64) vw->viewport_size.x;
	ui->view_matrix.m[1][1] = 1.0 / (f64) vw->viewport_size.y;
	ui->view_matrix.m[3][3] = scale_inv;
}

static void ui_draw_storage_quad(StorageQuad *out_sq, Quad *viewport_quad, Quad *grid_quad)
{
	Quad block_quad;
	quad_block_align(&block_quad, viewport_quad, GRID_BLOCK_SIZE);
	quad_intersect(&block_quad, &block_quad, grid_quad);

	storage_quad_from_quad(out_sq, &block_quad);
}

void ui_draw_fill(Ui *ui, View *vw, Grid *g, Quad *viewport_quad)
{
	glUseProgram(ui->fill_shader.program);

	glBindBuffer(GL_ARRAY_BUFFER, ui->buffers.vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui->buffers.fill_indices);

	ui_configure_attributes(ui);
	ui_update_view_matrix(ui, vw);

	glUniform2f(
			ui->fill_uniforms.gridSize,
			(f32) g->storage_quad.size.c,
			(f32) g->storage_quad.size.r);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui->textures.fill_colors);
	glUniform1i(ui->fill_uniforms.fillColors, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ui->textures.grid_styles);
	glUniform1i(ui->fill_uniforms.gridStyles, 1);

	//////////////////
	// Draw

	StorageQuad sq;
	ui_draw_storage_quad(&sq, viewport_quad, &g->quad);
	Hex sq_min_offset = hex_sub(sq.min, g->storage_quad.min);

	f64 trans_x = ui->translation_x;
	f64 trans_y = ui->translation_y;
	f64 size_x = vw->viewport_size.x;
	f64 size_y = vw->viewport_size.y;

	for (i32 r = 0; r < sq.size.r; r += UI_MESH_SIZE) {
		for (i32 c = 0; c < sq.size.c; c += UI_MESH_SIZE) {
			vec2 h = {
				(c + sq.min.c) << 1,
				r + sq.min.r,
			};
			vec2 v = mat2_multiply_v(&MESH_HEX_TO_POINT, h);

			ui->view_matrix.m[3][0] = (trans_x + v.x) / size_x;
			ui->view_matrix.m[3][1] = (trans_y + v.y) / size_y;

			glUniformMatrix4fv(
					ui->fill_uniforms.viewMatrix,
					1,
					GL_FALSE,
					(GLfloat*) &ui->view_matrix.m[0][0]);

			glUniform2f(
					ui->fill_uniforms.gridPositionOffset,
					(f32) (c + sq_min_offset.c),
					(f32) (r + sq_min_offset.r));

			glDrawElements(
					GL_TRIANGLES,
					ui->mesh.fill_indices_length,
					GL_UNSIGNED_SHORT,
					0);
		}
	}

}

void ui_update_styles(Ui *ui, Grid *g)
{
	(void) ui;

	glActiveTexture(GL_TEXTURE1);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			g->storage_quad.size.c,
			g->storage_quad.size.r,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			g->styles);
}

void ui_update_styles_in_quad(Ui *ui, Grid *g, Quad *quad)
{
	StorageQuad sq;
	storage_quad_from_quad(&sq, quad);
	i32 need_capacity = storage_quad_capacity(&sq);

	if (need_capacity > ui->styles_buffer_capacity) {
		if (need_capacity > ui->styles_buffer_capacity_max) {
			ui_update_styles(ui, g);

			return;
		}

		free(ui->styles_buffer);

		ui->styles_buffer = malloc(need_capacity * sizeof *ui->styles_buffer);
		ui->styles_buffer_capacity = need_capacity;
	}

	Hex min = hex_sub(sq.min, g->storage_quad.min);
	i32 storage_size_c = g->storage_quad.size.c;

	for (i32 r = 0; r < sq.size.r; ++r) {
		u8 *dest = &ui->styles_buffer[r * sq.size.c];
		i32 src_i = (r + min.r) * storage_size_c + min.c;
		const u8 *src = &g->styles[src_i];

		memcpy(dest, src, sq.size.c);
	}

	glActiveTexture(GL_TEXTURE1);

	glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			min.c,
			min.r,
			sq.size.c,
			sq.size.r,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			ui->styles_buffer);
}
