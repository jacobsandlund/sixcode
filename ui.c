#include <stdio.h>
#include "ui.h"

#define UI_HEX_MESH_SIZE 64

const char UI_VERTEX_SHADER_SOURCE[] =
"attribute vec4 position;\n"
"attribute vec2 gridPosition;\n"
"\n"
"uniform mat4 viewMatrix;\n"
"\n"
"uniform sampler2D fillColors;\n"
"uniform sampler2D gridStyles;\n"
"\n"
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	vec2 styleCoord = vec2(gridPosition.s / 64.0, gridPosition.t / 64.0);\n"
"	float style = 16.0 * texture2D(gridStyles, styleCoord).a + 16.0 * 0.5 / 256.0;\n"
"	color = texture2D(fillColors, vec2(style, 0.5));\n"
"	gl_Position = viewMatrix * position;\n"
"}\n";

const char UI_FRAGMENT_SHADER_SOURCE[] =
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	gl_FragColor = color;\n"
"}\n";

const u8 UI_FILL_COLORS[] = {
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
			fprintf(stderr, "%s:%d - An unacceptable value is specified for an enumerated argument.\n", filename, line);
			break;
		case GL_INVALID_VALUE:
			fprintf(stderr, "%s:%d - A numeric argument is out of range.\n", filename, line);
			break;
		case GL_INVALID_OPERATION:
			fprintf(stderr, "%s:%d - The specified operation is not allowed in the current state.\n", filename, line);
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			fprintf(stderr, "%s:%d - The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete.\n", filename, line);
			break;
		case GL_OUT_OF_MEMORY:
			fprintf(stderr, "%s:%d - There is not enough memory left to execute the command.\n", filename, line);
			break;
		default:
			fprintf(stderr, "%s:%d - Unknown GL Error %d\n", filename, line, (i32) error);
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
			fprintf(stderr, "Error compiling shader:\n%s\n", info_log);
			free(info_log);
		} else {
			fprintf(stderr, "Error compiling shader. Nothing in info log.\n");
		}

		ui_print_gl_error(__FILE__, __LINE__);
		glDeleteShader(shader);

		return 0;
	}

	return shader;
}

void ui_initialize(Ui *ui, Grid *g)
{
	////////////////////
	// clear

	ui->program = 0;
	ui->buffers.hex_mesh_vertices = 0;
	ui->buffers.hex_mesh_fill_indices = 0;
	ui->buffers.hex_mesh_stroke_indices = 0;
	ui->textures.fill_colors = 0;
	ui->textures.grid_styles = 0;

	////////////////////////
	// load/create + attach

	ui->vertex_shader = ui_load_shader(GL_VERTEX_SHADER, UI_VERTEX_SHADER_SOURCE);
	ui->fragment_shader = ui_load_shader(GL_FRAGMENT_SHADER, UI_FRAGMENT_SHADER_SOURCE);
	if (!ui->vertex_shader || !ui->fragment_shader) {
		ui_terminate(ui);
		return;
	}

	ui->program = glCreateProgram();
	if (!ui->program) {
		ui_print_gl_error(__FILE__, __LINE__);
		ui_terminate(ui);
		return;
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
			fprintf(stderr, "Error linking program:\n%s\n", info_log);
			free(info_log);
		} else {
			fprintf(stderr, "Error linking program. Nothing in info log.\n");
		}

		ui_print_gl_error(__FILE__, __LINE__);
		ui_terminate(ui);
		return;
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

	
	////////////////
	// textures

	glGenTextures(1, &ui->textures.fill_colors);
	glUniform1i(ui->locations.fillColors, 0);
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
			16,
			1,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			UI_FILL_COLORS);

	Hex grid_size = quad_size(g->quad);

	glGenTextures(1, &ui->textures.grid_styles);
	glUniform1i(ui->locations.gridStyles, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ui->textures.grid_styles);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			grid_size.c,
			grid_size.r,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			g->styles);
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
}

void ui_draw(Ui *ui, View *vw)
{
	glViewport(0, 0, vw->viewport_size.x, vw->viewport_size.y);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(ui->program);

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

	glUniformMatrix4fv(ui->locations.viewMatrix, 1, GL_FALSE, (GLfloat*) &vw->view_matrix.m[0][0]);

	glDrawElements(
			GL_TRIANGLES,
			ui->hex_mesh.fill_indices_length,
			GL_UNSIGNED_SHORT,
			0);
}
