#include "resource.h"

static void resource_noop_init(Resource *resource, uintptr_t options)
{
	resource->pointer = NULL;
}

static void resource_noop_destroy(Resource *resource)
{
	(void) resource;
}

const ResourceInitFn resource_default_init = resource_noop_init;
const ResourceDestroyFn resource_default_destroy = resource_noop_destroy;
