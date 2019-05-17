#ifndef SHADER_H
#define SHADER_H

#include <GLES3/gl3.h>
#include "sixcode.h"

typedef struct {
	GLuint vertex;
	GLuint fragment;
	GLuint program;
} ShaderProgram;

void shader_print_gl_error(const char *filename, int line);
GLuint shader_load(GLenum type, const char *shader_source, const char *filename, int line);

bool shader_program_create(ShaderProgram *s, GLuint vertex, GLuint fragment, const char *filename, int line);
bool shader_program_link(ShaderProgram *s, const char *filename, int line);
void shader_program_delete(ShaderProgram *s);

#endif // SHADER_H
