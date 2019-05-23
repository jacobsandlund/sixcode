#ifndef RENDER_LOOP_H
#define RENDER_LOOP_H

#include "spacetime.h"
#include "event-queue.h"
#include "renderer.h"
#include "world.h"

void render_loop_one_iteration(Renderer *r, EventQueue *eq, World *w);

#endif // RENDER_LOOP_H
