#include "test.h"
#include "resource/resource.c"
#include "resource/manager.c"

Test(ResourceDefault)
{
	Resource *resource = tmalloc(sizeof *resource);
	ResourceDefaultInitFn(resource, 0);
	_d(resource->pointer == NULL);

	i64 foo = 42;
	resource->pointer = &foo;

	ResourceDefaultDestroyFn(resource);

	_d(resource->pointer == NULL);
}

Test(resource_register_loaders)
{
	resource_register_loaders();

	_d(gResourceManager.loaders[ResourceTypeList].type == ResourceTypeList);
}
