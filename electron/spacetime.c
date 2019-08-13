#include "../src/core.c"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <stdarg.h>


//////////////////
// View

EMSCRIPTEN_KEEPALIVE View *web_view_malloc()
{
	View *vw = malloc(sizeof *vw);
	return vw;
}

EMSCRIPTEN_KEEPALIVE void web_view_initialize(View *vw, float width, float height, float translation_x, float translation_y, float scale)
{
	vec2 viewport_size = {width, height};
	vec2 translation = {translation_x, translation_y};
	view_initialize(vw, viewport_size, translation, scale);
}

EMSCRIPTEN_KEEPALIVE void web_view_zoom_at_screen_point(View *vw, float x, float y, float new_scale)
{
	vec2 v = {x, y};
	view_zoom_at_screen_point(vw, v, new_scale);
}

EMSCRIPTEN_KEEPALIVE void web_view_resize(View *vw, float width, float height)
{
	vec2 viewport_size = {width, height};
	view_resize(vw, viewport_size);
}

EMSCRIPTEN_KEEPALIVE void web_view_translate(View *vw, float delta_x, float delta_y)
{
	vec2 delta = {delta_x, delta_y};
	view_translate(vw, delta);
}

EMSCRIPTEN_KEEPALIVE void web_view_layout_set(View *vw, int layout)
{
	view_layout(vw, (ViewLayout) layout);
}

EMSCRIPTEN_KEEPALIVE int web_view_layout_get(View *vw)
{
	return (int) vw->layout;
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
	ivec2 h = {c, r};
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
	ui_initialize(ui);
}

EMSCRIPTEN_KEEPALIVE void web_ui_draw(Ui *ui, View *vw, Grid *g)
{
	ui_draw(ui, vw, g);
}

EMSCRIPTEN_KEEPALIVE void web_texture_update(Ui *ui, Grid *g)
{
	texture_update(&ui->grid_styles_texture, g);
}


//////////////////
// Core

EMSCRIPTEN_KEEPALIVE void web_core_grid_initialize(Grid *g)
{
	core_grid_initialize(g);
}

EMSCRIPTEN_KEEPALIVE void web_core_tick(Ui *ui, View *vw, Grid *g)
{
	core_tick(ui, vw, g);
}

EMSCRIPTEN_KEEPALIVE void web_core_toggle_hex_at_point(Ui *ui, View *vw, Grid *g, float x, float y)
{
	vec2 v = {x, y};
	core_toggle_hex_at_point(ui, vw, g, v);
}


//////////////////
// Main

#ifdef __EMSCRIPTEN__

void log_error(const char *format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
}

int main()
{
	EmscriptenWebGLContextAttributes attrs;
	emscripten_webgl_init_context_attributes(&attrs);

	attrs.majorVersion = 2;
	attrs.minorVersion = 0;
	//attrs.alpha = EM_FALSE;
	attrs.alpha = EM_TRUE;
	//attrs.depth = EM_FALSE;
	//attrs.stencil = EM_FALSE;
	//attrs.antialias = EM_FALSE;
	attrs.antialias = EM_TRUE;
	//attrs.premultipliedAlpha = EM_TRUE;
	//attrs.preserveDrawingBuffer = EM_FALSE;
	//attrs.failIfMajorPerformanceCaveat = EM_TRUE;
	//attrs.enableExtensionsByDefault = EM_FALSE;

	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);
	assert(context > 0);

	EMSCRIPTEN_RESULT res = emscripten_webgl_make_context_current(context);
	assert(res == EMSCRIPTEN_RESULT_SUCCESS);
	assert(emscripten_webgl_get_current_context() == context);

        EM_ASM(spacetime_initialized());

        return 0;
}

#endif
