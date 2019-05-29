#ifndef RenderLoop_h
#define RenderLoop_h

#include "spacetime.h"
#include "engine/event-queue.h"
// #include "renderer.h"
#include "engine/world.h"

void render_loop_one_iteration(Renderer *r, EventQueue *eq, World *w);

#endif // RenderLoop_h
