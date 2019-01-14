#ifndef __INTERFACE_JS_H__
#define __INTERFACE_JS_H__

#include "core.h"

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>
	#define EXPORT_JS EMSCRIPTEN_KEEPALIVE
#else
	#define EXPORT_JS
#endif

EXPORT_JS Layout *js_layout_create(f64 width, f64 height, f64 translation_x, f64 translation_y, f64 scale);
EXPORT_JS void js_layout_translate_by_delta(Layout *l, f64 delta_x, f64 delta_y);
EXPORT_JS void js_layout_zoom_at_point(Layout *l, f64 x, f64 y, f64 new_scale);
EXPORT_JS void js_layout_resize_viewport(Layout *l, f64 width, f64 height);

EXPORT_JS Grid *js_grid_create();

EXPORT_JS Mesh *js_mesh_create(i32 hex_capacity);
EXPORT_JS Point *js_mesh_points(Mesh *m);
EXPORT_JS i32 js_mesh_generate_hexes(Mesh *m, Layout *l, Grid *g);
EXPORT_JS StyledMesh *js_styled_mesh_create(i32 hex_capacity, i32 style_count);
EXPORT_JS i32 *js_styled_mesh_hex_style_indices(StyledMesh *sm);

EXPORT_JS void js_core_toggle_hex_at_point(Layout *l, Grid *g, f64 point_x, f64 point_y);
EXPORT_JS i32 js_core_styled_mesh_generate_hexes(StyledMesh *sm, Layout *l, Grid *g);

#endif // __INTERFACE_JS_H__
