#ifndef __CORE_H__
#define __CORE_H__

#include "sixcode.h"
#include "draw.h"
#include "grid.h"
#include "hex.h"
#include "matrix.h"
#include "mesh.h"
#include "quad.h"
#include "shader.h"
#include "ui.h"
#include "view.h"

i8 core_grid_expand_for_hex(Ui *ui, Grid *g, Hex h);
void core_toggle_hex_at_point(Ui *ui, View *vw, Grid *g, vec2 v);

#endif // __CORE_H__
