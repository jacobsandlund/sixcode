#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include "core.h"
#include "core.c"


//////////////////
// View

EXPORT_WEB View *web_view_malloc()
{
	View *vw = malloc(sizeof *vw);
	return vw;
}

EXPORT_WEB void web_view_initialize(View *vw, f32 width, f32 height, f32 translation_x, f32 translation_y, f32 scale)
{
	vec2 viewport_size = {width, height};
	vec2 translation = {translation_x, translation_y};
	return view_initialize(vw, viewport_size, translation, scale);
}

EXPORT_WEB void web_view_translate_by_delta(View *vw, f32 delta_x, f32 delta_y)
{
	vw->translation.x += delta_x;
	vw->translation.y += delta_y;
}

EXPORT_WEB void web_view_zoom_at_point(View *vw, f32 x, f32 y, f32 new_scale)
{
	vec2 v = {x, y};
	view_zoom_at_point(vw, v, new_scale);
}

EXPORT_WEB void web_view_resize(View *vw, f32 width, f32 height)
{
	vec2 viewport_size = {width, height};
	view_resize(vw, viewport_size);
}


////////////////
// Grid

EXPORT_WEB Grid *web_grid_malloc()
{
	Grid *g = malloc(sizeof *g);
	return g;
}

EXPORT_WEB void web_grid_initialize(Grid *g, i32 min_c, i32 min_r, i32 max_c, i32 max_r)
{
	Quad quad = {
		{min_c, min_r},
		{max_c, max_r},
	};
	grid_initialize(g, quad);
}

EXPORT_WEB void web_grid_set(Grid *g, i32 c, i32 r, u8 style)
{
	Hex h = {c, r};
	grid_set(g, h, style);
}


////////////////
// Ui

EXPORT_WEB Ui *web_ui_malloc()
{
	Ui *ui = malloc(sizeof *ui);
	return ui;
}

////////////////
// Main

#ifdef __EMSCRIPTEN__

int main()
{
	EmscriptenWebGLContextAttributes attrs;
	attrs.majorVersion = 1;
	emscripten_webgl_init_context_attributes(&attrs);
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);
	assert(context > 0);
	EMSCRIPTEN_RESULT res = emscripten_webgl_make_context_current(context);
	assert(res == EMSCRIPTEN_RESULT_SUCCESS);
	assert(emscripten_webgl_get_current_context() == context);

	EM_ASM( core_initialized() );

	return 0;
}

#endif
