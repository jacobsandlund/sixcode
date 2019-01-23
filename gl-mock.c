#include <GLES2/gl2.h>
#include "sixcode.h"

#define GL_MOCK_MAX_NUM_BUFFERS 32
#define GL_MOCK_MAX_NUM_PROGRAMS 4
#define GL_MOCK_MAX_NUM_SHADERS 8
#define GL_MOCK_MAX_NUM_TEXTURES 8

typedef struct {
	GLuint bound_buffers[2];
	const GLvoid *buffer_data[2];

	GLsizei viewport_width;
	GLsizei viewport_height;
	const GLfloat *uniform_matrix4_value;
	GLsizei draw_elements_count;
	GLsizei attib_pointer_stride;

	GLenum active_texture;
	i32 texture_width[GL_MOCK_MAX_NUM_TEXTURES];
	i32 texture_height[GL_MOCK_MAX_NUM_TEXTURES];
	const GLvoid *texture_data[GL_MOCK_MAX_NUM_TEXTURES];

	i32 buffer_i;
	i32 program_i;
	i32 shader_i;
	i32 texture_i;
	i32 uniform_i;
	i32 buffers[GL_MOCK_MAX_NUM_BUFFERS];
	i32 programs[GL_MOCK_MAX_NUM_PROGRAMS];
	i32 program_link_status[GL_MOCK_MAX_NUM_PROGRAMS];
	i32 shaders[GL_MOCK_MAX_NUM_SHADERS];
	i32 shader_attachments[GL_MOCK_MAX_NUM_SHADERS];
	i32 shader_compile_status[GL_MOCK_MAX_NUM_SHADERS];
	i32 textures[GL_MOCK_MAX_NUM_TEXTURES];
} GlMock;

GlMock GL_MOCK = {};

void gl_mock_initialize()
{
	GL_MOCK.buffer_i = 1;
	GL_MOCK.program_i = 1;
	GL_MOCK.shader_i = 1;
	GL_MOCK.texture_i = 1;
	GL_MOCK.uniform_i = 1;

	for (i32 i = 0; i < GL_MOCK_MAX_NUM_PROGRAMS; ++i) {
		GL_MOCK.program_link_status[i] = 0;
	}

	for (i32 i = 0; i < GL_MOCK_MAX_NUM_SHADERS; ++i) {
		GL_MOCK.shader_compile_status[i] = 0;
	}
}

void glActiveTexture(GLenum texture)
{
	GL_MOCK.active_texture = texture;
}

void glAttachShader(GLuint program, GLuint shader)
{
	(void) program;
	GL_MOCK.shader_attachments[shader] = 1;
}

void glBindAttribLocation(GLuint program, GLuint index, const GLchar *name)
{
	(void) program;
	(void) index;
	(void) name;
}

void glBindTexture(GLenum target, GLuint texture)
{
	(void) target;
	(void) texture;
}

static i32 buffer_target_i(GLenum target)
{
	return target == GL_ARRAY_BUFFER ? 0 : 1;
}

void glBindBuffer(GLenum target, GLuint buffer)
{
	GL_MOCK.bound_buffers[buffer_target_i(target)] = buffer;
}
	
void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
	(void) size;
	(void) usage;
	GL_MOCK.buffer_data[buffer_target_i(target)] = data;
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
	++GL_MOCK.shader_compile_status[shader];
}

GLuint glCreateProgram()
{
	GL_MOCK.programs[GL_MOCK.program_i] = 1;
	return GL_MOCK.program_i++;
}
	
GLuint glCreateShader()
{
	GL_MOCK.shaders[GL_MOCK.shader_i] = 1;
	return GL_MOCK.shader_i++;
}

void glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
	(void) n;
	GL_MOCK.buffers[buffers[0]] = 0;
}

void glDeleteTextures(GLsizei n, const GLuint *textures)
{
	(void) n;
	GL_MOCK.textures[textures[0]] = 0;
}

void glDeleteProgram(GLuint program)
{
	GL_MOCK.programs[program] = 0;
}

void glDeleteShader(GLuint shader)
{
	GL_MOCK.shaders[shader] = 0;
}

void glDetachShader(GLuint program, GLuint shader)
{
	(void) program;
	GL_MOCK.shader_attachments[shader] = 0;
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	(void) mode;
	(void) type;
	(void) indices;
	GL_MOCK.draw_elements_count = count;
}

void glEnableVertexAttribArray(GLuint index)
{
	(void) index;
}

void glGenBuffers(GLsizei n, GLuint *buffers)
{
	(void) n;
	GL_MOCK.buffers[GL_MOCK.buffer_i] = 1;
	buffers[0] = GL_MOCK.buffer_i++;
}

void glGenTextures(GLsizei n, GLuint *textures)
{
	(void) n;
	GL_MOCK.textures[GL_MOCK.texture_i] = 1;
	textures[0] = GL_MOCK.texture_i++;
}

GLenum glGetError()
{
	return GL_NO_ERROR;
}

void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
	(void) program;
	(void) maxLength;
	(void) length;
	(void) infoLog;
}

void glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
	switch (pname) {
	case GL_LINK_STATUS:
		params[0] = GL_MOCK.program_link_status[program];
		break;
	case GL_INFO_LOG_LENGTH:
		params[0] = 0;
		break;
	}
}

void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
	(void) shader;
	(void) maxLength;
	(void) length;
	(void) infoLog;
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
	switch (pname) {
	case GL_COMPILE_STATUS:
		params[0] = GL_MOCK.shader_compile_status[shader];
		break;
	case GL_INFO_LOG_LENGTH:
		params[0] = 0;
		break;
	}
}

GLint glGetUniformLocation(GLuint program, const GLchar *name)
{
	(void) program;
	(void) name;
	return GL_MOCK.uniform_i++;
}

void glLinkProgram(GLuint program)
{
	++GL_MOCK.program_link_status[program];
}
	
void glShaderSource(GLuint shader, GLsizei count, const GLchar * const *string, const GLint *length)
{
	(void) shader;
	(void) count;
	(void) string;
	(void) length;
}

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data)
{
	(void) target;
	(void) level;
	(void) internalformat;
	(void) border;
	(void) format;
	(void) type;

	i32 texture_i = GL_MOCK.active_texture - GL_TEXTURE0;
	GL_MOCK.texture_width[texture_i] = width;
	GL_MOCK.texture_height[texture_i] = height;
	GL_MOCK.texture_data[texture_i] = data;
}

void glTexParameteri(GLenum target, GLenum pname, GLint param)
{
	(void) target;
	(void) pname;
	(void) param;
}

void glUniform1i(GLint location, GLint v0)
{
	(void) location;
	(void) v0;
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	(void) location;
	(void) count;
	(void) transpose;
	GL_MOCK.uniform_matrix4_value = value;
}

void glUseProgram(GLuint program)
{
	(void) program;
}

void glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	(void) index;
	(void) v0;
	(void) v1;
	(void) v2;
	(void) v3;
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
	(void) index;
	(void) size;
	(void) type;
	(void) normalized;
	(void) pointer;
	GL_MOCK.attib_pointer_stride = stride;
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	(void) x;
	(void) y;
	GL_MOCK.viewport_width = width;
	GL_MOCK.viewport_height = height;
}
