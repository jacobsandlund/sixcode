#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "6code.h"
#include <GLES3/gl3.h>
#include "grid.h"
#include "quad.h"

#define TEXTURE_MAX_SIZE 4096

typedef struct {
	GLuint texture;
	u8 *buffer;
	int buffer_capacity;
	int buffer_capacity_max;
} Texture;

void texture_initialize(Texture *t, int buffer_capacity_max);
void texture_terminate(Texture *t);
void texture_update(Texture *t, Grid *g);
void texture_update_in_quad(Texture *t, Grid *g, Quad *quad);

#endif // __TEXTURE_H__
