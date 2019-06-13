#include "resource/resource.c"
#include "test.h"

Test(ResourceDefault)
{
    Resource *resource = tmalloc(sizeof *resource);
    ResourceDefaultInitFn(resource, 0);
    _d(resource->pointer == NULL);
    //=> 1

    i64 foo = 42;
    resource->pointer = &foo;

    ResourceDefaultDestroyFn(resource);

    _d(resource->pointer == NULL);
    //=> 1
}
