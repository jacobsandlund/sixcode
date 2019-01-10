#ifndef __INTERFACE_JS_H__
#define __INTERFACE_JS_H__

#include "core.h"

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>
	#define EXPORT_JS EMSCRIPTEN_KEEPALIVE
#else
	#define EXPORT_JS
#endif

EXPORT_JS Layout *js_layout_create(f64 scale, f64 origin_x, f64 origin_y);
EXPORT_JS void js_layout_set_scale(Layout *l, f64 scale);
EXPORT_JS void js_layout_set_origin(Layout *l, f64 origin_x, f64 origin_y);

EXPORT_JS Grid *js_grid_create();

EXPORT_JS Mesh *js_mesh_create(i32 hex_capacity);
EXPORT_JS Point *js_mesh_points(Mesh *m);
EXPORT_JS i32 js_mesh_generate_hexes(Mesh *m, Layout *l, Grid *g);

EXPORT_JS void js_core_toggle_hex_at_point(Layout *l, Grid *g, f64 point_x, f64 point_y);

#endif // __INTERFACE_JS_H__
