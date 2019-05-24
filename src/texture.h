#ifndef Texture_h
#define Texture_h

#include "spacetime.h"
#include <GLES3/gl3.h>
#include "grid.h"
#include "quad.h"

#define TextureMaxSize 4096

typedef struct {
	GLuint texture;
	u8 *buffer;
	i64 buffer_capacity;
	i64 buffer_capacity_max;
} Texture;

void texture_init(Texture *t, i64 buffer_capacity_max);
void texture_destroy(Texture *t);
void texture_update(Texture *t, Grid *g);
void texture_update_in_quad(Texture *t, Grid *g, Quad *quad);

#endif // Texture_h
