#include "resource.h"

static void *resource_noop_init(uintptr_t options)
{
	return NULL;
}

static void resource_noop_destroy(void *pointer)
{
	(void) pointer;
}

const ResourceInitFn resource_default_init = resource_noop_init;
const ResourceDestroyFn resource_default_destroy = resource_noop_destroy;
