#ifndef Renderer_h
#define Renderer_h

#include "spacetime.h"
#include "world.h"

typedef struct {
	void *os_renderer;
} Renderer;

void renderer_render(Renderer *r, World *w);

#endif // Renderer_h
