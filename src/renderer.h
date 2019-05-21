#ifndef RENDERER_H
#define RENDERER_H

#include "spacetime.h"
#include "view.h"
#include "world.h"

typedef struct {
	void *os_renderer;
} Renderer;

void renderer_render(Renderer *r, View *vw, World *w);

#endif // RENDERER_H
