#ifndef __SHADER_H__
#define __SHADER_H__

#include <GLES2/gl2.h>

typedef GLuint Shader;

typedef struct {
	Shader vertex;
	Shader fragment;
	GLuint program;
} ShaderProgram;

void shader_print_gl_error(const char *filename, int line);
Shader shader_load(GLenum type, const char *shader_source, const char *filename, i32 line);

i8 shader_program_create(ShaderProgram *s, const char *filename, i32 line);
i8 shader_program_link(ShaderProgram *s, const char *filename, i32 line);
void shader_program_delete(ShaderProgram *s);
void shader_program_delete_shaders(ShaderProgram *s);

#endif // __SHADER_H__
