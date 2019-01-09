#ifndef __INTERFACE_JS_H__
#define __INTERFACE_JS_H__

#include "sixcode.h"
#include "layout.h"
#include "point.h"
#include "mesh.h"
#include "grid.h"
#include "core.h"

// TODO:
#include "hex.c"
#include "layout.c"
#include "point.c"
#include "mesh.c"
#include "grid.c"
#include "core.c"
#include "quad.c"

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>
	#define EXPORT_JS EMSCRIPTEN_KEEPALIVE
#else
	#define EXPORT_JS
#endif

EXPORT_JS Layout *js_layout_create(f64 scale, f64 origin_x, f64 origin_y);
EXPORT_JS void js_layout_set_scale(Layout *l, f64 scale);
EXPORT_JS void js_layout_set_origin(Layout *l, f64 origin_x, f64 origin_y);

EXPORT_JS Grid *js_grid_create(u32 capacity);

EXPORT_JS Mesh *js_mesh_create(u32 point_capacity);
EXPORT_JS Point *js_mesh_points(Mesh *m);
EXPORT_JS u32 js_mesh_generate_hexes(Mesh *m, Layout *l, Grid *g);

EXPORT_JS void js_core_toggle_hex_at_point(Layout *l, Grid *g, f64 point_x, f64 point_y);

#endif // __INTERFACE_JS_H__
