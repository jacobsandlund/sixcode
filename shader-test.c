#include "test.h"
#include "glmock.c"
#include "shader.c"
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
	ShaderProgram s = {};

	// Bad create

	glmock_initialize();
	GLmock.shader_i = -1;
	GLmock.force_gl_error = GL_INVALID_OPERATION;

	s.vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	_d(s.vertex);
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> shader-test.c:75 - The specified operation is not allowed in the current state.
	//=>

	// Bad compile

	glmock_initialize();
	GLmock.shaders[1].compiled = -1;
	GLmock.force_gl_error = GL_OUT_OF_MEMORY;
	GLmock.force_info_log = "[Info Log] - Compilation failed";

	s.vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	_d(s.vertex);
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> Error compiling shader:
	//=> [Info Log] - Compilation failed
	//=> shader-test.c:89 - There is not enough memory left to execute the command.
	//=>

	// Success

	glmock_initialize();
	s.vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	s.fragment = shader_load(GL_FRAGMENT_SHADER, TEST_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);
	_dd(s.vertex, s.fragment);
	//=> 1, 2

	GLmockShader *vertex = &GLmock.shaders[s.vertex];
	GLmockShader *fragment = &GLmock.shaders[s.fragment];

	_dd(vertex->created, fragment->created);
	//=> 1, 1
	_d(vertex->shaderType == GL_VERTEX_SHADER);
	//=> 1
	_d(vertex->source == TEST_VERTEX_SHADER_SOURCE);
	//=> 1
	_d(fragment->shaderType == GL_FRAGMENT_SHADER);
	//=> 1
	_d(fragment->source == TEST_FRAGMENT_SHADER_SOURCE);
	//=> 1

	GLint compiled;
	_dd(vertex->compiled, fragment->compiled);
	//=> 1, 1
	glGetShaderiv(s.vertex, GL_COMPILE_STATUS, &compiled);
	_d(compiled);
	//=> 1
	glGetShaderiv(s.fragment, GL_COMPILE_STATUS, &compiled);
	_d(compiled);
	//=> 1

	// Delete

	shader_program_delete_shaders(&s);
	_dd(vertex->deleted, fragment->deleted);
	//=> 1, 1
}

TEST(shader_program_create_and_delete)
{
	ShaderProgram s = {};

	// No vertex or fragment shaders

	_d(shader_program_create(&s, __FILE__, __LINE__));
	//=> 0

	// Bad create

	glmock_initialize();
	GLmock.program_i = -1;
	GLmock.force_gl_error = GL_INVALID_OPERATION;
	s.vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	s.fragment = shader_load(GL_FRAGMENT_SHADER, TEST_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);

	_d(shader_program_create(&s, __FILE__, __LINE__));
	//=> 0
	_TEST_SIXCODE_ERROR();
	//=> shader-test.c:154 - The specified operation is not allowed in the current state.
	//=> Error creating program.
	//=>

	// Success

	glmock_initialize();
	s.vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	s.fragment = shader_load(GL_FRAGMENT_SHADER, TEST_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);

	_d(shader_program_create(&s, __FILE__, __LINE__));
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

	shader_program_delete(&s);

	_d(p->deleted);
	//=> 1
	_d(p->attached_vertex_shader);
	//=> 0
	_d(p->attached_fragment_shader);
	//=> 0

	shader_program_delete_shaders(&s);
}

TEST(shader_program_link)
{
	ShaderProgram s = {};

	// Bad link

	glmock_initialize();
	s.vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	s.fragment = shader_load(GL_FRAGMENT_SHADER, TEST_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);
	shader_program_create(&s, __FILE__, __LINE__);
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
	//=> shader-test.c:207 - A numeric argument is out of range.
	//=>

	// Success

	glmock_initialize();
	s.vertex = shader_load(GL_VERTEX_SHADER, TEST_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	s.fragment = shader_load(GL_FRAGMENT_SHADER, TEST_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);
	shader_program_create(&s, __FILE__, __LINE__);

	_d(shader_program_link(&s, __FILE__, __LINE__));
	//=> 1

	_d(GLmock.programs[s.program].linked);
	//=> 1
	glGetProgramiv(s.program, GL_LINK_STATUS, &linked);
	_d(linked);
	//=> 1

	shader_program_delete(&s);
	shader_program_delete_shaders(&s);
}
