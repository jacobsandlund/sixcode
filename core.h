#ifndef __CORE_H__
#define __CORE_H__

#include "bit-array.h"
#include "grid.h"
#include "hex.h"
#include "layout.h"
#include "mesh.h"
#include "point.h"
#include "quad.h"
#include "sixcode.h"

void core_toggle_hex_at_point(Layout *l, Grid *g, Point p);
i32 core_styled_mesh_style_fn(void *context, Hex h, void *data);

#endif // __CORE_H__
