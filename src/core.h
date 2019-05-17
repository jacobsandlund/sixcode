#ifndef CORE_H
#define CORE_H

#include "spacetime.h"
#include "grid.h"
#include "quad.h"
#include "ui.h"
#include "view.h"

void core_grid_initialize(Grid *g);
void core_tick(Ui *ui, View *vw, Grid *g);
void core_toggle_hex_at_point(Ui *ui, View *vw, Grid *g, vec2 v);

#endif // CORE_H
