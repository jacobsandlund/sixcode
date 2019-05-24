#include "test.h"
#include "resource-pool-allocator.c"

Test(resource_pool_allocator_allocate)
{
	ResourcePoolAllocator *rp = malloc(sizeof *rp);
	resource_pool_allocator_init(rp, 16);

	_d(rp->capacity);
	//=> 16
	_d(rp->allocated_size);
	//=> 0
	_d(rp->memory != NULL);
	//=> 1

	void *old_p = rp->memory;
	void *p = resource_pool_allocator_allocate(rp, 10);

	_d(p == rp->memory);
	//=> 1
	_d(rp->allocated_size);
	//=> 10

	old_p = p;
	p = resource_pool_allocator_allocate(rp, 4);

	_d(p != old_p);
	//=> 1
	_d(rp->allocated_size);
	//=> 14

	// Allocate over capacity

	_d(resource_pool_allocator_allocate(rp, 6));
	//=> 0

	_Log();
	//=> Resource pool allocator attempt to allocate 6 bytes with only 2 bytes remaining

	resource_pool_allocator_destroy(rp);
	free(rp);
}
