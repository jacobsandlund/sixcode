#include "../core.c"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <stdarg.h>

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
	vw->viewport_size = (vec2) {width, height};
	vw->translation = (vec2) {translation_x, translation_y};
	vw->scale = scale;
}

EMSCRIPTEN_KEEPALIVE void web_view_zoom_at_screen_point(View *vw, f32 x, f32 y, f32 new_scale)
{
	vec2 v = {x, y};
	view_zoom_at_screen_point(vw, v, new_scale);
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
// Ui All

EMSCRIPTEN_KEEPALIVE UiAll *web_ui_all_malloc()
{
	UiAll *ui = malloc(sizeof *ui);
	return ui;
}

EMSCRIPTEN_KEEPALIVE void web_ui_all_initialize(UiAll *ui)
{
	ui_all_initialize(ui, WEB_UI_STYLES_BUFFER_CAPACITY_MAX);
}

EMSCRIPTEN_KEEPALIVE UiGrid *web_ui_grid(UiAll *ui)
{
	return &ui->grid;
}

EMSCRIPTEN_KEEPALIVE void web_ui_all_draw(UiAll *ui, View *vw, Grid *g)
{
	ui_all_draw(ui, vw, g);
}


///////////////
// Ui Grid

EMSCRIPTEN_KEEPALIVE void web_ui_grid_update_styles(UiGrid *ui_grid, Grid *g)
{
	ui_grid_update_styles(ui_grid, g);
}


//////////////////
// Core

EMSCRIPTEN_KEEPALIVE void web_core_grid_initialize(Grid *g)
{
	Quad quad = {{-UI_GRID_MAX_TEXTURE_SIZE + 2, -UI_GRID_MAX_TEXTURE_SIZE / 2 + 1}, {UI_GRID_MAX_TEXTURE_SIZE - 3, UI_GRID_MAX_TEXTURE_SIZE / 2 - 2}};
	core_grid_initialize(g, &quad);
}

EMSCRIPTEN_KEEPALIVE void web_core_tick(UiAll *ui, View *vw, Grid *g)
{
	core_tick(ui, vw, g);
}

EMSCRIPTEN_KEEPALIVE void web_core_toggle_hex_at_point(UiAll *ui, View *vw, Grid *g, f32 x, f32 y)
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

	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);
	assert(context > 0);

	EMSCRIPTEN_RESULT res = emscripten_webgl_make_context_current(context);
	assert(res == EMSCRIPTEN_RESULT_SUCCESS);
	assert(emscripten_webgl_get_current_context() == context);

	EM_ASM( sixcode_initialized() );

	return 0;
}

#endif
