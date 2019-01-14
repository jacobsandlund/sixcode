#ifndef __CORE_H__
#define __CORE_H__

#include "sixcode.h"
#include "hex.h"
#include "point.h"
#include "layout.h"
#include "grid.h"
#include "bit-array.h"
#include "quad.h"
#include "mesh.h"

void core_toggle_hex_at_point(Layout *l, Grid *g, Point p);
void core_styled_mesh_generate_hexes(StyledMesh *sm, Layout *l, Grid *g);

#endif // __CORE_H__
