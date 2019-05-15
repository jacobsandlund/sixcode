#ifndef __CORE_H__
#define __CORE_H__

#include "6code.h"
#include "grid.h"
#include "quad.h"
#include "ui.h"
#include "view.h"

void core_grid_initialize(Grid *g);
void core_tick(Ui *ui, View *vw, Grid *g);
void core_toggle_hex_at_point(Ui *ui, View *vw, Grid *g, vec2 v);

#endif // __CORE_H__
