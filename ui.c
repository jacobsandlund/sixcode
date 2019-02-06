#include <stdio.h>
#include <string.h>
#include "ui.h"

#define UI_FILL_COLORS_COUNT 256
#define UI_FILL_COLOR_COMPONENTS_LENGTH 1024  // 256 * 4
#define UI_STYLES_BUFFER_CAPACITY_MIN 256

// 0.001953125 = 0.5 / 256
const char UI_VERTEX_SHADER_SOURCE[] =
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


void ui_print_gl_error(const char *filename, int line)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		switch (error) {
		case GL_INVALID_ENUM:
			SIXCODE_ERROR("%s:%d - An unacceptable value is specified for an enumerated argument.\n", filename, line);
			break;
		case GL_INVALID_VALUE:
			SIXCODE_ERROR("%s:%d - A numeric argument is out of range.\n", filename, line);
			break;
		case GL_INVALID_OPERATION:
			SIXCODE_ERROR("%s:%d - The specified operation is not allowed in the current state.\n", filename, line);
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			SIXCODE_ERROR("%s:%d - The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete.\n", filename, line);
			break;
		case GL_OUT_OF_MEMORY:
			SIXCODE_ERROR("%s:%d - There is not enough memory left to execute the command.\n", filename, line);
			break;
		default:
			SIXCODE_ERROR("%s:%d - Unknown GL Error %d\n", filename, line, (i32) error);
			break;
		}
	}
}

static GLuint ui_load_shader(GLenum type, const char *shader_source)
{
	GLuint shader = glCreateShader(type);
	if (!shader) {
		ui_print_gl_error(__FILE__, __LINE__);
		return 0;
	}

	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 1) {
			char *info_log = malloc(info_log_length * sizeof *info_log);
			glGetShaderInfoLog(shader, info_log_length, NULL, info_log);
			SIXCODE_ERROR("Error compiling shader:\n%s\n", info_log);
			free(info_log);
		} else {
			SIXCODE_ERROR("Error compiling shader. Nothing in info log.\n");
		}

		ui_print_gl_error(__FILE__, __LINE__);
		glDeleteShader(shader);

		return 0;
	}

	return shader;
}

i8 ui_initialize(Ui *ui, i32 styles_buffer_capacity_max)
{
	////////////////////////
	// load/create + attach

	ui->vertex_shader = ui_load_shader(GL_VERTEX_SHADER, UI_VERTEX_SHADER_SOURCE);
	ui->fragment_shader = ui_load_shader(GL_FRAGMENT_SHADER, UI_FRAGMENT_SHADER_SOURCE);
	if (!ui->vertex_shader || !ui->fragment_shader) {
		glDeleteShader(ui->vertex_shader);
		glDeleteShader(ui->fragment_shader);

		return 0;
	}

	ui->program = glCreateProgram();
	if (!ui->program) {
		ui_print_gl_error(__FILE__, __LINE__);
		SIXCODE_ERROR("Error creating program.\n");
		glDeleteShader(ui->vertex_shader);
		glDeleteShader(ui->fragment_shader);

		return 0;
	}

	glAttachShader(ui->program, ui->vertex_shader);
	glAttachShader(ui->program, ui->fragment_shader);

	////////////////////
	// locations

	UiLocations *loc = &ui->locations;
	loc->position     = 0;
	loc->gridPosition = 1;

	glBindAttribLocation(ui->program, loc->position, "position");
	glBindAttribLocation(ui->program, loc->gridPosition, "gridPosition");

	glLinkProgram(ui->program);

	loc->viewMatrix = glGetUniformLocation(ui->program, "viewMatrix");
	loc->gridSize = glGetUniformLocation(ui->program, "gridSize");
	loc->gridPositionOffset = glGetUniformLocation(ui->program, "gridPositionOffset");
	loc->fillColors = glGetUniformLocation(ui->program, "fillColors");
	loc->gridStyles = glGetUniformLocation(ui->program, "gridStyles");

	////////////////////
	// link

	GLint linked;
	glGetProgramiv(ui->program, GL_LINK_STATUS, &linked);

	if (!linked) {
		GLint info_log_length = 0;
		glGetProgramiv(ui->program, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 1) {
			char *info_log = malloc(info_log_length * sizeof *info_log);
			glGetProgramInfoLog(ui->program, info_log_length, NULL, info_log);
			SIXCODE_ERROR("Error linking program:\n%s\n", info_log);
			free(info_log);
		} else {
			SIXCODE_ERROR("Error linking program. Nothing in info log.\n");
		}

		ui_print_gl_error(__FILE__, __LINE__);

		glDetachShader(ui->program, ui->vertex_shader);
		glDetachShader(ui->program, ui->fragment_shader);
		glDeleteProgram(ui->program);
		glDeleteShader(ui->vertex_shader);
		glDeleteShader(ui->fragment_shader);

		return 0;
	}

	glUseProgram(ui->program);

	//////////////////
	// mesh + buffers

	mesh_initialize(&ui->hex_mesh, UI_HEX_MESH_SIZE, UI_HEX_MESH_SIZE);

	UiBuffers *buf = &ui->buffers;

	glGenBuffers(1, &buf->hex_mesh_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, buf->hex_mesh_vertices);
	glBufferData(
			GL_ARRAY_BUFFER,
			ui->hex_mesh.vertices_length * sizeof *ui->hex_mesh.vertices,
			ui->hex_mesh.vertices,
			GL_STATIC_DRAW);

	glGenBuffers(1, &buf->hex_mesh_fill_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->hex_mesh_fill_indices);
	glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			ui->hex_mesh.fill_indices_length * sizeof *ui->hex_mesh.fill_indices,
			ui->hex_mesh.fill_indices,
			GL_STATIC_DRAW);

	glGenBuffers(1, &buf->hex_mesh_stroke_indices);
	// TODO
	
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
	if (ui->program) {
		glDetachShader(ui->program, ui->vertex_shader);
		glDetachShader(ui->program, ui->fragment_shader);
		glDeleteProgram(ui->program);
	}

	glDeleteShader(ui->vertex_shader);
	glDeleteShader(ui->fragment_shader);

	mesh_terminate(&ui->hex_mesh);

	glDeleteBuffers(1, &ui->buffers.hex_mesh_vertices);
	glDeleteBuffers(1, &ui->buffers.hex_mesh_fill_indices);
	glDeleteBuffers(1, &ui->buffers.hex_mesh_stroke_indices);

	glDeleteTextures(1, &ui->textures.fill_colors);
	glDeleteTextures(1, &ui->textures.grid_styles);

	free(ui->styles_buffer);
}

void ui_draw(Ui *ui, View *vw, Grid *g)
{
	/////////////////////
	// Global state

	glViewport(0, 0, vw->viewport_size.x, vw->viewport_size.y);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(ui->program);

	/////////////////////
	// Attributes

	glBindBuffer(GL_ARRAY_BUFFER, ui->buffers.hex_mesh_vertices);

	glVertexAttribPointer(
			ui->locations.position,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof ui->hex_mesh.vertices[0],
			0);
	glEnableVertexAttribArray(ui->locations.position);

	glVertexAttrib4f(ui->locations.position, 0.0f, 0.0f, 0.0f, 1.0f);

	glVertexAttribPointer(
			ui->locations.gridPosition,
			2,
			GL_BYTE,
			GL_FALSE,
			sizeof ui->hex_mesh.vertices[0],
			(GLvoid *) (2 * sizeof ui->hex_mesh.vertices[0].x));
	glEnableVertexAttribArray(ui->locations.gridPosition);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui->buffers.hex_mesh_fill_indices);

	///////////////////
	// Uniforms

	glUniform2f(
			ui->locations.gridSize,
			(f32) g->storage_quad.size.c,
			(f32) g->storage_quad.size.r);

	//////////////////
	// Textures

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui->textures.fill_colors);
	glUniform1i(ui->locations.fillColors, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ui->textures.grid_styles);
	glUniform1i(ui->locations.gridStyles, 1);

	//////////////////
	// View matrix

	f64 size_x = vw->viewport_size.x;
	f64 size_y = vw->viewport_size.y;
	f64 scale_inv = 1.0 / vw->scale;
	f64 translation_x = -vw->translation.x * scale_inv * 2.0;
	f64 translation_y = vw->translation.y * scale_inv * 2.0;

	ui->view_matrix.m[0][0] = 1.0 / size_x;
	ui->view_matrix.m[1][1] = 1.0 / size_y;
	ui->view_matrix.m[3][3] = scale_inv;

	//////////////////
	// Quad

	Quad q;
	view_viewport_to_quad(vw, &q);
	quad_block_align(&q, &q, GRID_BLOCK_SIZE);
	quad_intersect(&q, &q, &g->quad);

	StorageQuad sq;
	storage_quad_from_quad(&sq, &q);

	Hex sq_min_offset = hex_sub(sq.min, g->storage_quad.min);

	//////////////////
	// Draw

	for (i32 r = 0; r < sq.size.r; r += UI_HEX_MESH_SIZE) {
		for (i32 c = 0; c < sq.size.c; c += UI_HEX_MESH_SIZE) {
			vec2 h = {
				(c + sq.min.c) << 1,
				r + sq.min.r,
			};
			vec2 v = mat2_multiply_v(&MESH_HEX_TO_POINT, h);

			ui->view_matrix.m[3][0] = (translation_x + v.x) / size_x;
			ui->view_matrix.m[3][1] = (translation_y + v.y) / size_y;

			glUniformMatrix4fv(
					ui->locations.viewMatrix,
					1,
					GL_FALSE,
					(GLfloat*) &ui->view_matrix.m[0][0]);

			glUniform2f(
					ui->locations.gridPositionOffset,
					(f32) (c + sq_min_offset.c),
					(f32) (r + sq_min_offset.r));

			glDrawElements(
					GL_TRIANGLES,
					ui->hex_mesh.fill_indices_length,
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
