#include "shader.c"
#include "test.h"
#include "glmock.c"
#include <string.h>

TEST(shader_print_gl_error)
{
	glmock_initialize();

	GLmock.force_gl_error = GL_INVALID_ENUM;
	shader_print_gl_error(__FILE__, __LINE__);
	_TEST_SIXCODE_ERROR();
	//=> shader-test.c:11 - An unacceptable value is specified for an enumerated argument.
	//=>

	GLmock.force_gl_error = GL_INVALID_VALUE;
	shader_print_gl_error(__FILE__, __LINE__);
	_TEST_SIXCODE_ERROR();
	//=> shader-test.c:17 - A numeric argument is out of range.
	//=>

	GLmock.force_gl_error = GL_INVALID_OPERATION;
	shader_print_gl_error(__FILE__, __LINE__);
	_TEST_SIXCODE_ERROR();
	//=> shader-test.c:23 - The specified operation is not allowed in the current state.
	//=>

	GLmock.force_gl_error = GL_INVALID_FRAMEBUFFER_OPERATION;
	shader_print_gl_error(__FILE__, __LINE__);
	_TEST_SIXCODE_ERROR();
	//=> shader-test.c:29 - The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete.
	//=>

	GLmock.force_gl_error = GL_OUT_OF_MEMORY;
	shader_print_gl_error(__FILE__, __LINE__);
	_TEST_SIXCODE_ERROR();
	//=> shader-test.c:35 - There is not enough memory left to execute the command.
	//=>

	GLmock.force_gl_error = 1;
	shader_print_gl_error(__FILE__, __LINE__);
	_TEST_SIXCODE_ERROR();
	//=> shader-test.c:41 - Unknown GL Error 1
	//=>
}

const char TEST_VERTEX_SHADER_SOURCE[] =
"attribute vec4 position;\n"
"\n"
"uniform mat4 viewMatrix;\n"
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
"	gl_Position = viewMatrix * position;\n"
"}\n";

const char TEST_FRAGMENT_SHADER_SOURCE[] =
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	gl_FragColor = color;\n"
"}\n";

TEST(shader_load_and_delete)
{
	// Bad create

	glmock_initialize();
	GLmock.shader_i = -1;
	GLmock.force_gl_error = GL_INVALID_OPERATION;

	_d(shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> shader-test.c:73 - The specified operation is not allowed in the current state.
	//=>

	// Bad compile

	glmock_initialize();
	GLmock.shaders[1].compiled = -1;
	GLmock.force_gl_error = GL_OUT_OF_MEMORY;
	GLmock.force_info_log = "[Info Log] - Compilation failed";

	_d(shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error compiling shader:
	//=> [Info Log] - Compilation failed
	//=> shader-test.c:86 - There is not enough memory left to execute the command.
	//=>

	// Success

	glmock_initialize();
	GLuint vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	GLuint fragment = shader_load(GL_FRAGMENT_SHADER, TEST_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);
	_dd(vertex, fragment);
	//=> 1, 2

	GLmockShader *v = &GLmock.shaders[vertex];
	GLmockShader *f = &GLmock.shaders[fragment];

	_dd(v->created, f->created);
	//=> 1, 1
	_d(v->shaderType == GL_VERTEX_SHADER);
	//=> 1
	_d(v->source == TEST_VERTEX_SHADER_SOURCE);
	//=> 1
	_d(f->shaderType == GL_FRAGMENT_SHADER);
	//=> 1
	_d(f->source == TEST_FRAGMENT_SHADER_SOURCE);
	//=> 1

	GLint compiled;
	_dd(v->compiled, f->compiled);
	//=> 1, 1
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
	_d(compiled);
	//=> 1
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiled);
	_d(compiled);
	//=> 1

	// Delete

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	_dd(v->deleted, f->deleted);
	//=> 1, 1
}

TEST(shader_program_create_and_delete)
{
	GLuint vertex;
	GLuint fragment;
	ShaderProgram s = {};

	// No vertex or fragment shaders

	_d(shader_program_create(&s, 0, 0, __FILE__, __LINE__));
	//=> 0

	// Bad create

	glmock_initialize();
	GLmock.program_i = -1;
	GLmock.force_gl_error = GL_INVALID_OPERATION;
	vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	fragment = shader_load(GL_FRAGMENT_SHADER, TEST_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);

	_d(shader_program_create(&s, vertex, fragment, __FILE__, __LINE__));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> shader-test.c:154 - The specified operation is not allowed in the current state.
	//=> Error creating program.
	//=>

	// Success

	glmock_initialize();
	vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	fragment = shader_load(GL_FRAGMENT_SHADER, TEST_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);

	_d(shader_program_create(&s, vertex, fragment, __FILE__, __LINE__));
	//=> 1

	_d(s.program);
	//=> 1

	GLmockProgram *p = &GLmock.programs[s.program];
	_d(p->created);
	//=> 1
	_dd(p->attached_vertex_shader, s.vertex);
	//=> 1, 1
	_dd(p->attached_fragment_shader, s.fragment);
	//=> 2, 2

	// Delete

	shader_program_delete(&s);

	_d(p->deleted);
	//=> 1
	_d(p->attached_vertex_shader);
	//=> 0
	_d(p->attached_fragment_shader);
	//=> 0

	glDeleteShader(s.vertex);
	glDeleteShader(s.fragment);

	_d(GLmock.shaders[s.vertex].deleted);
	//=> 1
	_d(GLmock.shaders[s.fragment].deleted);
	//=> 1
}

TEST(shader_program_link)
{
	GLuint vertex;
	GLuint fragment;
	ShaderProgram s = {};

	// Bad link

	glmock_initialize();
	vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	fragment = shader_load(GL_FRAGMENT_SHADER, TEST_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);
	shader_program_create(&s, vertex, fragment, __FILE__, __LINE__);
	GLmock.programs[s.program].linked = -1;
	GLmock.force_gl_error = GL_INVALID_VALUE;
	GLmock.force_info_log = "[Info Log] Link program failed";

	_d(shader_program_link(&s, __FILE__, __LINE__));
	//=> 0

	GLint linked;
	glGetProgramiv(s.program, GL_LINK_STATUS, &linked);
	_d(linked);
	//=> 0
	_d(GLmock.programs[s.program].deleted);
	//=> 1
	_TEST_SIXCODE_ERROR();
	//=> Error linking program:
	//=> [Info Log] Link program failed
	//=> shader-test.c:217 - A numeric argument is out of range.
	//=>

	// Success

	glmock_initialize();
	vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	fragment = shader_load(GL_FRAGMENT_SHADER, TEST_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);
	shader_program_create(&s, vertex, fragment, __FILE__, __LINE__);

	_d(shader_program_link(&s, __FILE__, __LINE__));
	//=> 1

	_d(GLmock.programs[s.program].linked);
	//=> 1
	glGetProgramiv(s.program, GL_LINK_STATUS, &linked);
	_d(linked);
	//=> 1

	shader_program_delete(&s);
	glDeleteShader(s.vertex);
	glDeleteShader(s.fragment);
}
