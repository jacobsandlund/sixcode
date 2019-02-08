#include <stdio.h>
#include "shader.h"

void shader_print_gl_error(const char *filename, i32 line)
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

Shader shader_load(GLenum type, const char *shader_source, const char *filename, i32 line)
{
	Shader shader = glCreateShader(type);

	if (!shader) {
		shader_print_gl_error(filename, line);

		return 0;
	}

	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint info_log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 1) {
			char *info_log = malloc(info_log_length * sizeof *info_log);
			glGetShaderInfoLog(shader, info_log_length, NULL, info_log);
			SIXCODE_ERROR("Error compiling shader:\n%s\n", info_log);
			free(info_log);
		} else {
			SIXCODE_ERROR("Error compiling shader. Nothing in info log.\n");
		}

		shader_print_gl_error(filename, line);
		glDeleteShader(shader);

		return 0;
	}

	return shader;
}

i8 shader_program_create(ShaderProgram *s, const char *filename, i32 line)
{
	if (!s->vertex || !s->fragment) {
		return 0;
	}

	s->program = glCreateProgram();

	if (!s->program) {
		shader_print_gl_error(filename, line);
		SIXCODE_ERROR("Error creating program.\n");

		return 0;
	}

	glAttachShader(s->program, s->vertex);
	glAttachShader(s->program, s->fragment);

	return 1;
}

i8 shader_program_link(ShaderProgram *s, const char *filename, i32 line)
{
	glLinkProgram(s->program);

	GLint linked;
	glGetProgramiv(s->program, GL_LINK_STATUS, &linked);

	if (!linked) {
		GLint info_log_length;
		glGetProgramiv(s->program, GL_INFO_LOG_LENGTH, &info_log_length);

		if (info_log_length > 1) {
			char *info_log = malloc(info_log_length * sizeof *info_log);
			glGetProgramInfoLog(s->program, info_log_length, NULL, info_log);
			SIXCODE_ERROR("Error linking program:\n%s\n", info_log);
			free(info_log);
		} else {
			SIXCODE_ERROR("Error linking program. Nothing in info log.\n");
		}

		shader_print_gl_error(filename, line);
		shader_program_delete(s);

		return 0;
	}

	return 1;
}

void shader_program_delete(ShaderProgram *s)
{
	if (s->program) {
		glDetachShader(s->program, s->vertex);
		glDetachShader(s->program, s->fragment);
		glDeleteProgram(s->program);
	}
}

void shader_program_delete_shaders(ShaderProgram *s)
{
	glDeleteShader(s->vertex);
	glDeleteShader(s->fragment);
}
