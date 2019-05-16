#include <GLES3/gl3.h>
#include <string.h>
#include "6code.h"

#define GLMOCK_MAX_BUFFERS 32
#define GLMOCK_MAX_PROGRAMS 10
#define GLMOCK_MAX_SHADERS 20
#define GLMOCK_MAX_TEXTURES 16
#define GLMOCK_MAX_ATTRIBUTES 16
#define GLMOCK_MAX_UNIFORMS GLMOCK_MAX_ATTRIBUTES
#define GLMOCK_MAX_TEXTURE_PARAMETERS 4

typedef struct {
	bool created;
	bool deleted;
	bool compiled;
	bool force_compile_error;
	GLenum shaderType;
	const char *source;
} GLmockShader;

typedef struct {
	const char *name;
	GLint size;
	GLenum type;
	GLsizei stride;
	GLsizei offset;
	GLuint divisor;
	bool enabled_vertex_attrib_array;

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
	bool created;
	bool deleted;
	bool linked;
	bool force_link_error;
	int attached_vertex_shader;
	int attached_fragment_shader;

	int uniform_i;
	int attributes_i;
	GLmockUniform uniforms[GLMOCK_MAX_ATTRIBUTES];
	GLmockAttribute attributes[GLMOCK_MAX_ATTRIBUTES];
} GLmockProgram;

typedef struct {
	bool created;
	bool deleted;
	const GLvoid *data;
	GLsizeiptr size;
	GLenum usage;
} GLmockBuffer;

typedef struct {
	bool created;
	bool deleted;
	int width;
	int height;
	GLenum format;
	GLenum type;
	const GLvoid *data;
	GLint parameters[GLMOCK_MAX_TEXTURE_PARAMETERS];
	int xoffset;
	int yoffset;
} GLmockTexture;

typedef struct {
	GLsizei viewport_width;
	GLsizei viewport_height;

	GLenum draw_elements_mode;
	GLenum draw_elements_type;
	GLsizei draw_elements_count;
	GLsizei draw_elements_instanced_primcount;

	GLenum draw_arrays_mode;
	GLsizei draw_arrays_count;

	GLenum enabled_capability;
	GLenum disabled_capability;
	GLenum blend_source_factor;
	GLenum blend_destination_factor;

	GLenum force_gl_error;
	bool force_create_program_error;
	bool force_create_shader_error;
	const char *force_info_log;

	int buffer_i;
	int program_i;
	int shader_i;
	int texture_i;

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

static int glmock_buffer_target_i(GLenum target)
{
	return target == GL_ARRAY_BUFFER ? 0 : 1;
}

static int glmock_texture_i()
{
	return GLmock.active_texture - GL_TEXTURE0;
}

static int glmock_texture_parameter_i(GLenum pname)
{
	return pname - GL_TEXTURE_MAG_FILTER;
}

GLint glmock_get_tex_parameter(GLuint texture, GLenum pname)
{
	int param_i = glmock_texture_parameter_i(pname);
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

void glBindTexture(GLenum target, GLuint texture)
{
	(void) target;
	GLmock.bound_textures[glmock_texture_i()] = texture;
}

void glBindBuffer(GLenum target, GLuint buffer)
{
	GLmock.bound_buffers[glmock_buffer_target_i(target)] = buffer;
}

void glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	GLmock.blend_source_factor = sfactor;
	GLmock.blend_destination_factor = dfactor;
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
	GLmock.shaders[shader].compiled = !GLmock.shaders[shader].force_compile_error;
}

GLuint glCreateProgram()
{
	if (GLmock.force_create_program_error) {
		return 0;
	}

	GLmock.program_i++;
	GLmock.programs[GLmock.program_i].created = true;
	return GLmock.program_i;
}
	
GLuint glCreateShader(GLenum shaderType)
{
	if (GLmock.force_create_shader_error) {
		return 0;
	}

	GLmock.shader_i++;
	GLmock.shaders[GLmock.shader_i].created = true;
	GLmock.shaders[GLmock.shader_i].shaderType = shaderType;
	return GLmock.shader_i;
}

void glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
	(void) n;
	GLmock.buffers[buffers[0]].deleted = true;
}

void glDeleteTextures(GLsizei n, const GLuint *textures)
{
	(void) n;
	GLmock.textures[textures[0]].deleted = true;
}

void glDeleteProgram(GLuint program)
{
	GLmock.programs[program].deleted = true;
}

void glDeleteShader(GLuint shader)
{
	GLmock.shaders[shader].deleted = true;
}

void glDetachShader(GLuint program, GLuint shader)
{
	if (GLmock.shaders[shader].shaderType == GL_VERTEX_SHADER) {
		GLmock.programs[program].attached_vertex_shader = 0;
	} else {
		GLmock.programs[program].attached_fragment_shader = 0;
	}
}

void glDisable(GLenum capability)
{
	GLmock.disabled_capability = capability;
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	(void) first;
	GLmock.draw_arrays_mode = mode;
	GLmock.draw_arrays_count += count;
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	(void) indices;
	GLmock.draw_elements_mode = mode;
	GLmock.draw_elements_type = type;
	GLmock.draw_elements_count += count;
}

void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount)
{
	(void) indices;
	GLmock.draw_elements_mode = mode;
	GLmock.draw_elements_type = type;
	GLmock.draw_elements_count += primcount * count;
	GLmock.draw_elements_instanced_primcount += primcount;
}

void glEnableVertexAttribArray(GLuint index)
{
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	p->attributes[index].enabled_vertex_attrib_array = true;
}

void glEnable(GLenum capability)
{
	GLmock.enabled_capability = capability;
}

void glGenBuffers(GLsizei n, GLuint *buffers)
{
	(void) n;
	GLmock.buffer_i++;
	GLmock.buffers[GLmock.buffer_i].created = true;
	buffers[0] = GLmock.buffer_i;
}

void glGenTextures(GLsizei n, GLuint *textures)
{
	(void) n;
	GLmock.texture_i++;
	GLmock.textures[GLmock.texture_i].created = true;
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

GLint glGetAttribLocation(GLuint program, const GLchar *name)
{
	GLmockProgram *p = &GLmock.programs[program];
	p->attributes_i++;
	p->attributes[p->attributes_i].name = name;
	return p->attributes_i;
}

GLint glGetUniformLocation(GLuint program, const GLchar *name)
{
	GLmockProgram *p = &GLmock.programs[program];
	p->uniform_i++;
	p->uniforms[p->uniform_i].name = name;
	return p->uniform_i;
}

void glLinkProgram(GLuint program)
{
	GLmock.programs[program].linked = !GLmock.programs[program].force_link_error;
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
	int param_i = glmock_texture_parameter_i(pname);
	GLmock.textures[texture_i].parameters[param_i] = param;
}

void glUniform1i(GLint location, GLint v0)
{
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	GLmockUniform *uniform = &p->uniforms[location];
	uniform->iv0 = v0;
}

void glUniform1f(GLint location, GLfloat v0)
{
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	GLmockUniform *uniform = &p->uniforms[location];
	uniform->fv0 = v0;
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

void glVertexAttribDivisor(GLuint index, GLuint divisor)
{
	GLmockProgram *p = &GLmock.programs[GLmock.using_program];
	GLmockAttribute *attrib = &p->attributes[index];
	attrib->divisor = divisor;
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
