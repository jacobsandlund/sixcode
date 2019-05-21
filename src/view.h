#ifndef VIEW_H
#define VIEW_H

#include "spacetime.h"

typedef struct {
	void *os_view;

	float2 viewport_size;

	float2 drag_from_vector;
	float2 drag_to_vector;
	bool dragging;
} View;

#endif // VIEW_H
