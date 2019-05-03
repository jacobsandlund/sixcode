#ifndef __CORE_H__
#define __CORE_H__

#include "sixcode.h"
#include "grid.h"
#include "quad.h"
#include "ui-all.h"
#include "view.h"

void core_grid_initialize(Grid *g, Quad *quad);
void core_tick(UiAll *ui, View *vw, Grid *g);
bool core_grid_expand_for_hex(Grid *g, ivec2 h);
void core_toggle_hex_at_point(UiAll *ui, View *vw, Grid *g, vec2 v);

#endif // __CORE_H__
