#include "test.h"
#include "texture.c"
#include "glmock.c"
#include "grid.c"
#include "quad.c"

TEST(texture)
{
	Texture *t = malloc(sizeof *t);

	glmock_initialize();
	texture_initialize(t, 1024);

	//////////////////////
	// texture

	_d(t->texture);
	//=> 1

	GLmockTexture *texture = &GLmock.textures[t->texture];

	_d(glmock_get_tex_parameter(t->texture, GL_TEXTURE_WRAP_S) == GL_CLAMP_TO_EDGE);
	//=> 1
	_d(glmock_get_tex_parameter(t->texture, GL_TEXTURE_WRAP_T) == GL_CLAMP_TO_EDGE);
	//=> 1
	_d(glmock_get_tex_parameter(t->texture, GL_TEXTURE_MAG_FILTER) == GL_NEAREST);
	//=> 1
	_d(glmock_get_tex_parameter(t->texture, GL_TEXTURE_MIN_FILTER) == GL_NEAREST);
	//=> 1

	_d(texture->created);
	//=> 1

	//////////////////////
	// buffer

	_d(t->buffer != 0);
	//=> 1
	_d(t->buffer_capacity);
	//=> 256
	_d(t->buffer_capacity_max);
	//=> 1024

	//////////////////////
	// terminate

	texture_terminate(t);

	_d(texture->deleted);
	//=> 1

	free(t);
}

TEST(texture_update)
{
	Grid *g = malloc(sizeof *g);
	Texture *t = malloc(sizeof *t);

	glmock_initialize();
	grid_initialize(g);
	texture_initialize(t, 0);

	texture_update(t, g);

	GLmockTexture *texture = &GLmock.textures[t->texture];

	_dd(texture->width, texture->height);
	//=> 4096, 4096
	_d(texture->format == GL_ALPHA);
	//=> 1
	_d(texture->type == GL_UNSIGNED_BYTE);
	//=> 1
	_d(texture->data == g->styles);
	//=> 1

	_d(GLmock.bound_textures[0] == t->texture);
	//=> 1

	grid_terminate(g);
	texture_terminate(t);

	free(g);
	free(t);
}

TEST(texture_update_in_quad)
{
	Quad quad;
	SizeQuad sq;

	Grid *g = malloc(sizeof *g);
	Texture *t = malloc(sizeof *t);

	glmock_initialize();
	grid_initialize(g);
	texture_initialize(t, 2500);

	_d(t->buffer_capacity);
	//=> 256

	texture_update(t, g);

	// Under the current capacity
	quad = (Quad) {{10, 20}, {31, 59}};
	quad_to_size_quad(&sq, &quad);
	_i2(ivec2_sub(sq.min, g->size_quad.min));
	//=> 2058, 2068
	_i2(sq.size);
	//=> 22, 40
	
	texture_update_in_quad(t, g, &quad);

	GLmockTexture *texture = &GLmock.textures[t->texture];

	_dd(texture->xoffset, texture->yoffset);
	//=> 2058, 2068
	_dd(texture->width, texture->height);
	//=> 22, 40
	_d(texture->format == GL_ALPHA);
	//=> 1
	_d(texture->type == GL_UNSIGNED_BYTE);
	//=> 1
	_d(texture->data == t->buffer);
	//=> 1
	_d(t->buffer_capacity);
	//=> 880

	// Under/equal the max capacity
	quad = (Quad) {{10, 10}, {59, 59}};
	quad_to_size_quad(&sq, &quad);
	_i2(sq.size);
	//=> 50, 50
	_d(size_quad_capacity(&sq));
	//=> 2500

	texture_update_in_quad(t, g, &quad);

	_d(t->buffer_capacity);
	//=> 2500
	_dd(texture->width, texture->height);
	//=> 50, 50

	// Over the max capacity
	quad = (Quad) {{10, 10}, {59, 60}};
	quad_to_size_quad(&sq, &quad);
	_i2(ivec2_sub(sq.min, g->size_quad.min));
	//=> 2058, 2058
	_i2(sq.size);
	//=> 50, 51
	_i2(g->size_quad.size);
	//=> 4096, 4096

	texture_update_in_quad(t, g, &quad);

	_d(t->buffer_capacity);
	//=> 2500
	_dd(texture->width, texture->height);
	//=> 4096, 4096

	grid_terminate(g);
	texture_terminate(t);

	free(g);
	free(t);
}
