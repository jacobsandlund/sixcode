#ifndef __CORE_H__
#define __CORE_H__

#include "sixcode.h"
#include "grid.h"
#include "quad.h"
#include "ui-all.h"
#include "view.h"

void core_grid_initialize(Grid *g);
void core_tick(UiAll *ui, View *vw, Grid *g);
void core_toggle_hex_at_point(UiAll *ui, View *vw, Grid *g, vec2 v);

#endif // __CORE_H__
