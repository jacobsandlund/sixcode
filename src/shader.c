#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

void shader_print_gl_error(const char *filename, int line)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		switch (error) {
		case GL_INVALID_ENUM:
			SPACETIME_ERROR("%s:%d - An unacceptable value is specified for an enumerated argument.\n", filename, line);
			break;
		case GL_INVALID_VALUE:
			SPACETIME_ERROR("%s:%d - A numeric argument is out of range.\n", filename, line);
			break;
		case GL_INVALID_OPERATION:
			SPACETIME_ERROR("%s:%d - The specified operation is not allowed in the current state.\n", filename, line);
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			SPACETIME_ERROR("%s:%d - The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete.\n", filename, line);
			break;
		case GL_OUT_OF_MEMORY:
			SPACETIME_ERROR("%s:%d - There is not enough memory left to execute the command.\n", filename, line);
			break;
		default:
			SPACETIME_ERROR("%s:%d - Unknown GL Error %d\n", filename, line, (int) error);
			break;
		}
	}
}

GLuint shader_load(GLenum type, const char *shader_source, const char *filename, int line)
{
	GLuint shader = glCreateShader(type);

	if (!shader) {
		shader_print_gl_error(filename, line);

		return 0;
	}

	glShaderSource(shader, 1, &shader_source, 0);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint info_log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 1) {
			char *info_log = malloc(info_log_length * sizeof *info_log);
			glGetShaderInfoLog(shader, info_log_length, 0, info_log);
			SPACETIME_ERROR("Error compiling shader:\n%s\n", info_log);
			free(info_log);
		} else {
			SPACETIME_ERROR("Error compiling shader. Nothing in info log.\n");
		}

		shader_print_gl_error(filename, line);
		glDeleteShader(shader);

		return 0;
	}

	return shader;
}

bool shader_program_create(ShaderProgram *s, GLuint vertex, GLuint fragment, const char *filename, int line)
{
	if (!vertex || !fragment) {
		return false;
	}

	s->vertex = vertex;
	s->fragment = fragment;
	s->program = glCreateProgram();

	if (!s->program) {
		shader_print_gl_error(filename, line);
		SPACETIME_ERROR("Error creating program.\n");

		return false;
	}

	glAttachShader(s->program, s->vertex);
	glAttachShader(s->program, s->fragment);

	return true;
}

bool shader_program_link(ShaderProgram *s, const char *filename, int line)
{
	glLinkProgram(s->program);

	GLint linked;
	glGetProgramiv(s->program, GL_LINK_STATUS, &linked);

	if (!linked) {
		GLint info_log_length;
		glGetProgramiv(s->program, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 1) {
			char *info_log = malloc(info_log_length * sizeof *info_log);
			glGetProgramInfoLog(s->program, info_log_length, 0, info_log);
			SPACETIME_ERROR("Error linking program:\n%s\n", info_log);
			free(info_log);
		} else {
			SPACETIME_ERROR("Error linking program. Nothing in info log.\n");
		}

		shader_print_gl_error(filename, line);
		shader_program_delete(s);

		return false;
	}

	return true;
}

void shader_program_delete(ShaderProgram *s)
{
	if (s->program) {
		glDetachShader(s->program, s->vertex);
		glDetachShader(s->program, s->fragment);
		glDeleteProgram(s->program);
	}
}
