#ifndef CORE_H
#define CORE_H

#include "spacetime.h"
#include "renderer.h"
#include "view.h"
#include "world.h"

void core_loop_tick(Renderer *r, View *vw, World *w);

#endif // CORE_H
