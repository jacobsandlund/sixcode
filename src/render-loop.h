#ifndef RenderLoop_h
#define RenderLoop_h

#include "spacetime.h"
#include "event-queue.h"
#include "renderer.h"
#include "world.h"

void render_loop_one_iteration(Renderer *r, EventQueue *eq, World *w);

#endif // RenderLoop_h
