#include "texture.h"
#include <stdlib.h>
#include <string.h>

#define TEXTURE_BUFFER_CAPACITY_MIN 256

void texture_init(Texture *t, i64 buffer_capacity_max)
{
	//////////////////////
	// texture

	glGenTextures(1, &t->texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//////////////////////
	// buffer

	if (buffer_capacity_max < TEXTURE_BUFFER_CAPACITY_MIN) {
		buffer_capacity_max = TEXTURE_BUFFER_CAPACITY_MIN;
	}

	t->buffer = malloc(TEXTURE_BUFFER_CAPACITY_MIN * sizeof *t->buffer);
	t->buffer_capacity = TEXTURE_BUFFER_CAPACITY_MIN;
	t->buffer_capacity_max = buffer_capacity_max;
}

void texture_destroy(Texture *t)
{
	glDeleteTextures(1, &t->texture);
	free(t->buffer);
}

void texture_update(Texture *t, Grid *g)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t->texture);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			g->size_quad.size.x,
			g->size_quad.size.y,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			g->styles);
}

void texture_update_in_quad(Texture *t, Grid *g, Quad *quad)
{
	SizeQuad sq;
	quad_to_size_quad(&sq, quad);
	i64 need_capacity = size_quad_capacity(&sq);

	if (need_capacity > t->buffer_capacity) {
		if (need_capacity > t->buffer_capacity_max) {
			texture_update(t, g);

			return;
		}

		free(t->buffer);

		t->buffer = malloc(need_capacity * sizeof *t->buffer);
		t->buffer_capacity = need_capacity;
	}

	ivec2 min = ivec2_sub(sq.min, g->size_quad.min);
	i64 storage_size_x = g->size_quad.size.x;

	for (i64 r = 0; r < sq.size.y; r++) {
		u8 *dest = &t->buffer[r * sq.size.x];
		i64 src_i = (r + min.y) * storage_size_x + min.x;
		const u8 *src = &g->styles[src_i];

		memcpy(dest, src, sq.size.x);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t->texture);

	glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			min.x,
			min.y,
			sq.size.x,
			sq.size.y,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			t->buffer);
}
