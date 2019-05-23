#ifndef Shader_h
#define Shader_h

#include <GLES3/gl3.h>
#include "spacetime.h"

typedef struct {
	GLuint vertex;
	GLuint fragment;
	GLuint program;
} ShaderProgram;

void shader_print_gl_error(const char *filename, i64 line);
GLuint shader_load(GLenum type, const char *shader_source, const char *filename, i64 line);

bool shader_program_create(ShaderProgram *s, GLuint vertex, GLuint fragment, const char *filename, i64 line);
bool shader_program_link(ShaderProgram *s, const char *filename, i64 line);
void shader_program_delete(ShaderProgram *s);

#endif // Shader_h
