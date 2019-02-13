#ifndef __SHADER_H__
#define __SHADER_H__

#include <GLES2/gl2.h>
#include "sixcode.h"

typedef struct {
	GLuint vertex;
	GLuint fragment;
	GLuint program;
} ShaderProgram;

void shader_print_gl_error(const char *filename, int line);
GLuint shader_load(GLenum type, const char *shader_source, const char *filename, i32 line);

i8 shader_program_create(ShaderProgram *s, GLuint vertex, GLuint fragment, const char *filename, i32 line);
i8 shader_program_link(ShaderProgram *s, const char *filename, i32 line);
void shader_program_delete(ShaderProgram *s);

#endif // __SHADER_H__
