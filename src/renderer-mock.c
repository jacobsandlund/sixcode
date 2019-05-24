#include "renderer.h"

typedef struct {
	i64 num_render_calls;
} RendererMock;

RendererMock *renderer_mock(Renderer *r)
{
	return (RendererMock *) r->os_renderer;
}

void renderer_mock_init(Renderer *r)
{
	RendererMock *rm = malloc(sizeof *rm);
	*rm = (RendererMock) {};

	r->os_renderer = (void *) rm;
}

void renderer_mock_destroy(Renderer *r)
{
	free(r->os_renderer);
}

void renderer_render(Renderer *r, World *w)
{
	(void) w;
	renderer_mock(r)->num_render_calls++;
}
