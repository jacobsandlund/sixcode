#include "interface-js.h"

EXPORT_JS Layout *js_layout_create(f64 width, f64 height, f64 translation_x, f64 translation_y, f64 scale)
{
	Point viewport_size = {.x = width, .y = height};
	Point translation = {.x = translation_x, .y = translation_y};
	return layout_create(LAYOUT_POINTY, viewport_size, translation, scale);
}

EXPORT_JS void js_layout_translate_by_delta(Layout *l, f64 delta_x, f64 delta_y)
{
	l->translation.x += delta_x;
	l->translation.y += delta_y;
}

EXPORT_JS void js_layout_zoom_at_point(Layout *l, f64 x, f64 y, f64 new_scale)
{
	Point p = {.x = x, .y = y};
	layout_zoom_at_point(l, p, new_scale);
}

EXPORT_JS void js_layout_resize_viewport(Layout *l, f64 width, f64 height)
{
	l->viewport_size.x = width;
	l->viewport_size.y = height;
}


EXPORT_JS Grid *js_grid_create()
{
	return grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
}


EXPORT_JS Mesh *js_mesh_create(i32 hex_capacity)
{
	return mesh_create(hex_capacity);
}

EXPORT_JS Point *js_mesh_points(Mesh *m)
{
	return m->points;
}

EXPORT_JS i32 js_mesh_generate_hexes(Mesh *m, Layout *l, Grid *g)
{
	return mesh_generate_hexes(m, l, g);
}


EXPORT_JS void js_core_toggle_hex_at_point(Layout *l, Grid *g, f64 point_x, f64 point_y)
{
	Point p = {.x = point_x, .y = point_y};
	core_toggle_hex_at_point(l, g, p);
}


#ifdef __EMSCRIPTEN__

int main()
{
	EM_ASM( core_initialized() );
	return 0;
}

#endif
