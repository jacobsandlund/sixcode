#ifndef __CORE_H__
#define __CORE_H__

#include "sixcode.h"
#include "bit-array.h"
#include "grid.h"
#include "hex.h"
#include "matrix.h"
#include "mesh.h"
#include "quad.h"
#include "ui.h"
#include "view.h"

void core_toggle_hex_at_point(Ui *ui, View *vw, Grid *g, vec2 v);

#endif // __CORE_H__
