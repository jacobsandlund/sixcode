#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include "../core.h"
#include "../core.c"

#define WEB_UI_STYLES_BUFFER_CAPACITY_MAX 1048576	// 1 MB


//////////////////
// View

EMSCRIPTEN_KEEPALIVE View *web_view_malloc()
{
	View *vw = malloc(sizeof *vw);
	return vw;
}

EMSCRIPTEN_KEEPALIVE void web_view_initialize(View *vw, f32 width, f32 height, f32 translation_x, f32 translation_y, f32 scale)
{
	vec2 viewport_size = {width, height};
	vec2 translation = {translation_x, translation_y};
	return view_initialize(vw, viewport_size, translation, scale);
}

EMSCRIPTEN_KEEPALIVE void web_view_zoom_at_point(View *vw, f32 x, f32 y, f32 new_scale)
{
	vec2 v = {x, y};
	view_zoom_at_point(vw, v, new_scale);
}

EMSCRIPTEN_KEEPALIVE void web_view_resize(View *vw, f32 width, f32 height)
{
	vec2 viewport_size = {width, height};
	view_resize(vw, viewport_size);
}

EMSCRIPTEN_KEEPALIVE void web_view_translate(View *vw, f32 delta_x, f32 delta_y)
{
	vec2 delta = {delta_x, delta_y};
	view_translate(vw, delta);
}


////////////////
// Grid

EMSCRIPTEN_KEEPALIVE Grid *web_grid_malloc()
{
	Grid *g = malloc(sizeof *g);
	return g;
}

EMSCRIPTEN_KEEPALIVE void web_grid_set(Grid *g, i32 c, i32 r, u8 style)
{
	Hex h = {c, r};
	grid_set(g, h, style);
}


////////////////
// Ui

EMSCRIPTEN_KEEPALIVE Ui *web_ui_malloc()
{
	Ui *ui = malloc(sizeof *ui);
	return ui;
}

EMSCRIPTEN_KEEPALIVE void web_ui_initialize(Ui *ui)
{
	ui_initialize(ui, WEB_UI_STYLES_BUFFER_CAPACITY_MAX);
}

EMSCRIPTEN_KEEPALIVE void web_ui_update_styles(Ui *ui, Grid *g)
{
	ui_update_styles(ui, g);
}

EMSCRIPTEN_KEEPALIVE void web_ui_draw(Ui *ui, View *vw, Grid *g)
{
	ui_draw(ui, vw, g);
}


//////////////////
// Core

EMSCRIPTEN_KEEPALIVE void web_core_grid_initialize(Grid *g)
{
	core_grid_initialize(g);
}

EMSCRIPTEN_KEEPALIVE void web_core_toggle_hex_at_point(Ui *ui, View *vw, Grid *g, f32 x, f32 y)
{
	vec2 v = {x, y};
	core_toggle_hex_at_point(ui, vw, g, v);
}


//////////////////
// Main

#ifdef __EMSCRIPTEN__

int main()
{
	EmscriptenWebGLContextAttributes attrs;
	emscripten_webgl_init_context_attributes(&attrs);
	attrs.majorVersion = 1;
	attrs.minorVersion = 0;

	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);
	assert(context > 0);

	EMSCRIPTEN_RESULT res = emscripten_webgl_make_context_current(context);
	assert(res == EMSCRIPTEN_RESULT_SUCCESS);
	assert(emscripten_webgl_get_current_context() == context);

	EM_ASM( core_initialized() );

	return 0;
}

#endif
