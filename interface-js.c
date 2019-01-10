#include "interface-js.h"

EXPORT_JS Layout *js_layout_create(f64 scale, f64 origin_x, f64 origin_y)
{
	Point origin = {.x = origin_x, .y = origin_y};
	return layout_create(LAYOUT_POINTY, scale, origin);
}

EXPORT_JS void js_layout_set_scale(Layout *l, f64 scale)
{
	l->scale = scale;
}

EXPORT_JS void js_layout_set_origin(Layout *l, f64 origin_x, f64 origin_y)
{
	Point origin = {.x = origin_x, .y = origin_y};
	l->origin = origin;
}


EXPORT_JS Grid *js_grid_create()
{
	return grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
}


EXPORT_JS Mesh *js_mesh_create(u32 hex_capacity)
{
	return mesh_create(hex_capacity);
}

EXPORT_JS Point *js_mesh_points(Mesh *m)
{
	return m->points;
}

EXPORT_JS u32 js_mesh_generate_hexes(Mesh *m, Layout *l, Grid *g)
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
