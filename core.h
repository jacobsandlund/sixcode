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
i32 core_styled_mesh_style_fn(void *context, Hex h, void *data);

#endif // __CORE_H__
