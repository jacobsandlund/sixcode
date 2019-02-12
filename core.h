#ifndef __CORE_H__
#define __CORE_H__

#include "sixcode.h"
#include "grid.h"
#include "hex.h"
#include "matrix.h"
#include "mesh.h"
#include "quad.h"
#include "shader.h"
#include "ui-all.h"
#include "ui-fill.h"
#include "ui-grid.h"
#include "ui-points.h"
#include "ui-stroke.h"
#include "view.h"

i8 core_grid_expand_for_hex(UiAll *ui, Grid *g, Hex h);
void core_toggle_hex_at_point(UiAll *ui, View *vw, Grid *g, vec2 v);

#endif // __CORE_H__
