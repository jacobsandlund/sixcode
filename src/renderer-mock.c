#include "renderer.h"

typedef struct {
	int num_render_calls;
} RendererMock;

RendererMock *renderer_mock(Renderer *r)
{
	return (RendererMock *) r->os_renderer;
}

void renderer_mock_initialize(Renderer *r)
{
	RendererMock *rm = malloc(sizeof *rm);
	*rm = (RendererMock) {};

	r->os_renderer = (void *) rm;
}

void renderer_mock_terminate(Renderer *r)
{
	free(r->os_renderer);
}

void renderer_render(Renderer *r, View *vw, World *w)
{
	(void) vw;
	(void) w;
	renderer_mock(r)->num_render_calls++;
}
