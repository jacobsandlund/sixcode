#include "view.h"

void view_mock_initialize(View *vw, float2 viewport_size)
{
	vw->viewport_size = viewport_size;
}

void view_mock_terminate(View *r)
{
	(void) r;
}
