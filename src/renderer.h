#ifndef RENDERER_H
#define RENDERER_H

#include "spacetime.h"
#include "world.h"

typedef struct {
	void *os_renderer;
} Renderer;

void renderer_render(Renderer *r, World *w);

#endif // RENDERER_H
