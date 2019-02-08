#include <GLES2/gl2.h>
#include <string.h>
#include "sixcode.h"

#define GLMOCK_MAX_BUFFERS 32
#define GLMOCK_MAX_PROGRAMS 10
#define GLMOCK_MAX_SHADERS 20
#define GLMOCK_MAX_TEXTURES 16
#define GLMOCK_MAX_ATTRIBUTES 16
#define GLMOCK_MAX_UNIFORMS GLMOCK_MAX_ATTRIBUTES
#define GLMOCK_MAX_TEXTURE_PARAMETERS 4

typedef struct {
	i8 created;
	i8 deleted;
	i8 compiled;
	GLenum shaderType;
	const char *source;
} GLmockShader;

typedef struct {
	const char *name;
	GLint size;
	GLenum type;
	GLsizei stride;
	GLsizei offset;
	i8 enabled_vertex_attrib_array;

	GLfloat v0;
	GLfloat v1;
	GLfloat v2;
	GLfloat v3;
} GLmockAttribute;

typedef struct {
	const char *name;
	const GLfloat *matrix4fv;
	GLint iv0;
	GLfloat fv0;
	GLfloat fv1;
	GLfloat fv2;
	GLfloat fv3;
} GLmockUniform;

typedef struct {
	i8 created;
	i8 deleted;
	i8 linked;
	i32 attached_vertex_shader;
	i32 attached_fragment_shader;

	i32 uniform_i;
	GLmockUniform uniforms[GLMOCK_MAX_ATTRIBUTES];
	GLmockAttribute attributes[GLMOCK_MAX_ATTRIBUTES];
} GLmockProgram;

typedef struct {
	i8 created;
	i8 deleted;
	const GLvoid *data;
	GLsizeiptr size;
	GLenum usage;
} GLmockBuffer;

typedef struct {
	i8 created;
	i8 deleted;
	i32 width;
	i32 height;
	GLenum format;
	GLenum type;
	const GLvoid *data;
	GLint parameters[GLMOCK_MAX_TEXTURE_PARAMETERS];
	i32 xoffset;
	i32 yoffset;
} GLmockTexture;

typedef struct {
	GLsizei viewport_width;
	GLsizei viewport_height;

	GLenum draw_elements_mode;
	GLenum draw_elements_type;
	GLsizei draw_elements_count;

	GLenum force_gl_error;
	const char *force_info_log;

	i32 buffer_i;
	i32 program_i;
	i32 shader_i;
	i32 texture_i;

	GLenum active_texture;
	GLuint using_program;
	GLuint bound_buffers[2];
	GLuint bound_textures[GLMOCK_MAX_TEXTURES];

	GLmockBuffer buffers[GLMOCK_MAX_BUFFERS];
	GLmockProgram programs[GLMOCK_MAX_PROGRAMS];
	GLmockShader shaders[GLMOCK_MAX_SHADERS];
	GLmockTexture textures[GLMOCK_MAX_TEXTURES];
} GLmockGlobal;

GLmockGlobal GLmock = {};

void glmock_initialize()
{
	GLmock = (GLmockGlobal) {};
}

static i32 glmock_buffer_target_i(GLenum target)
{
	return target == GL_ARRAY_BUFFER ? 0 : 1;
}

static i32 glmock_texture_i()
{
	return GLmock.active_texture - GL_TEXTURE0;
}

static i32 glmock_texture_parameter_i(GLenum pname)
{
	return pname - GL_TEXTURE_MAG_FILTER;
}

GLint glmock_get_tex_parameter(GLuint texture, GLenum pname)
{
	i32 param_i = glmock_texture_parameter_i(pname);
	return GLmock.textures[texture].parameters[param_i];
}

void glActiveTexture(GLenum texture)
{
	GLmock.active_texture = texture;
}

void glAttachShader(GLuint program, GLuint shader)
{
	if (GLmock.shaders[shader].shaderType == GL_VERTEX_SHADER) {
		GLmock.programs[program].attached_vertex_shader = shader;
	} else {
		GLmock.programs[program].attached_fragment_shader = shader;
	}
}

void glBindAttribLocation(GLuint program, GLuint index, const GLchar *name)
{
	GLmock.programs[program].attributes[index].name = name;
}

void glBindTexture(GLenum target, GLuint texture)
{
	(void) target;
	GLmock.bound_textures[glmock_texture_i()] = texture;
}

void glBindBuffer(GLenum target, GLuint buffer)
{
	GLmock.bound_buffers[glmock_buffer_target_i(target)] = buffer;
}
	
void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
	GLuint buffer_i = GLmock.bound_buffers[glmock_buffer_target_i(target)];
	GLmockBuffer *buffer = &GLmock.buffers[buffer_i];
	buffer->data = data;
	buffer->size = size;
	buffer->usage = usage;
}

void glClear(GLbitfield mask)
{
	(void) mask;
}

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	(void) red;
	(void) green;
	(void) blue;
	(void) alpha;
}

void glCompileShader(GLuint shader)
{
	++GLmock.shaders[shader].compiled;
}

GLuint glCreateProgram()
{
	++GLmock.program_i;
	GLmock.programs[GLmock.program_i].created = 1;
	return GLmock.program_i;
}
	
GLuint glCreateShader(GLenum shaderType)
{
	++GLmock.shader_i;
	GLmock.shaders[GLmock.shader_i].created = 1;
	GLmock.shaders[GLmock.shader_i].shaderType = shaderType;
	return GLmock.shader_i;
}

void glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
	(void) n;
	GLmock.buffers[buffers[0]].deleted = 1;
}

void glDeleteTextures(GLsizei n, const GLuint *textures)
{
	(void) n;
	GLmock.textures[textures[0]].deleted = 1;
}

void glDeleteProgram(GLuint program)
{
	GLmock.programs[program].deleted = 1;
}

void glDeleteShader(GLuint shader)
{
	GLmock.shaders[shader].deleted = 1;
}

void glDetachShader(GLuint program, GLuint shader)
{
	if (GLmock.shaders[shader].shaderType == GL_VERTEX_SHADER) {
		GLmock.programs[program].attached_vertex_shader = 0;
	} else {
		GLmock.programs[program].attached_fragment_shader = 0;
	}
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	(void) indices;
	GLmock.draw_elements_mode = mode;
	GLmock.draw_elements_type = type;
	GLmock.draw_elements_count += count;
}

void glEnableVertexAttribArray(GLuint index)
{
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	p->attributes[index].enabled_vertex_attrib_array = 1;
}

void glGenBuffers(GLsizei n, GLuint *buffers)
{
	(void) n;
	++GLmock.buffer_i;
	GLmock.buffers[GLmock.buffer_i].created = 1;
	buffers[0] = GLmock.buffer_i;
}

void glGenTextures(GLsizei n, GLuint *textures)
{
	(void) n;
	++GLmock.texture_i;
	GLmock.textures[GLmock.texture_i].created = 1;
	textures[0] = GLmock.texture_i;
}

GLenum glGetError()
{
	if (GLmock.force_gl_error) {
		GLenum error = GLmock.force_gl_error;
		GLmock.force_gl_error = 0;

		return error;
	} else {
		return GL_NO_ERROR;
	}
}

void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
	(void) program;
	(void) length;

	if (GLmock.force_info_log) {
		memcpy(infoLog, GLmock.force_info_log, maxLength);
	}
}

void glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
	switch (pname) {
	case GL_LINK_STATUS:
		params[0] = GLmock.programs[program].linked;
		break;
	case GL_INFO_LOG_LENGTH:
		if (GLmock.force_info_log) {
			params[0] = strlen(GLmock.force_info_log) + 1;
		} else {
			params[0] = 0;
		}

		break;
	}
}

void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
	(void) shader;
	(void) length;

	if (GLmock.force_info_log) {
		memcpy(infoLog, GLmock.force_info_log, maxLength);
	}
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
	switch (pname) {
	case GL_COMPILE_STATUS:
		params[0] = GLmock.shaders[shader].compiled;
		break;
	case GL_INFO_LOG_LENGTH:
		if (GLmock.force_info_log) {
			params[0] = strlen(GLmock.force_info_log) + 1;
		} else {
			params[0] = 0;
		}

		break;
	}
}

GLint glGetUniformLocation(GLuint program, const GLchar *name)
{
	GLmockProgram *p = &GLmock.programs[program];
	++p->uniform_i;
	p->uniforms[p->uniform_i].name = name;
	return p->uniform_i;
}

void glLinkProgram(GLuint program)
{
	++GLmock.programs[program].linked;
}
	
void glShaderSource(GLuint shader, GLsizei count, const GLchar * const *string, const GLint *length)
{
	(void) count;
	(void) length;
	GLmock.shaders[shader].source = (const char *) *string;
}

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data)
{
	(void) target;
	(void) level;
	(void) internalformat;
	(void) border;

	GLuint texture_i = GLmock.bound_textures[glmock_texture_i()];
	GLmockTexture *t = &GLmock.textures[texture_i];
	t->width = width;
	t->height = height;
	t->format = format;
	t->type = type;
	t->data = data;
}

void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * data)
{
	(void) target;
	(void) level;

	GLuint texture_i = GLmock.bound_textures[glmock_texture_i()];
	GLmockTexture *t = &GLmock.textures[texture_i];
	t->width = width;
	t->height = height;
	t->format = format;
	t->type = type;
	t->data = data;
	t->xoffset = xoffset;
	t->yoffset = yoffset;
}

void glTexParameteri(GLenum target, GLenum pname, GLint param)
{
	(void) target;
	GLuint texture_i = GLmock.bound_textures[glmock_texture_i()];
	i32 param_i = glmock_texture_parameter_i(pname);
	GLmock.textures[texture_i].parameters[param_i] = param;
}

void glUniform1i(GLint location, GLint v0)
{
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	GLmockUniform *uniform = &p->uniforms[location];
	uniform->iv0 = v0;
}

void glUniform2f(GLint location, GLfloat v0, GLfloat v1)
{
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	GLmockUniform *uniform = &p->uniforms[location];
	uniform->fv0 = v0;
	uniform->fv1 = v1;
}

void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	GLmockUniform *uniform = &p->uniforms[location];
	uniform->fv0 = v0;
	uniform->fv1 = v1;
	uniform->fv2 = v2;
	uniform->fv3 = v3;
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	(void) transpose;
	(void) count;
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	GLmockUniform *uniform = &p->uniforms[location];
	uniform->matrix4fv = value;
}

void glUseProgram(GLuint program)
{
	GLmock.using_program = program;
}

void glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	GLmockAttribute *attrib = &p->attributes[index];
	attrib->v0 = v0;
	attrib->v1 = v1;
	attrib->v2 = v2;
	attrib->v3 = v3;
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
	(void) normalized;
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	GLmockAttribute *attrib = &p->attributes[index];
	attrib->size = size;
	attrib->type = type;
	attrib->stride = stride;
	attrib->offset = (GLsizei) pointer;
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	(void) x;
	(void) y;
	GLmock.viewport_width = width;
	GLmock.viewport_height = height;
}
