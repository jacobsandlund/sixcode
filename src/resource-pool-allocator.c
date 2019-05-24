#include <inttypes.h>
#include <stdlib.h>
#include "resource-pool-allocator.h"

void resource_pool_allocator_init(ResourcePoolAllocator *rp, u64 capacity)
{
	rp->memory = malloc(capacity);
	rp->capacity = capacity;
	rp->allocated_size = 0;
}

void resource_pool_allocator_destroy(ResourcePoolAllocator *rp)
{
	free(rp->memory);
}

void *resource_pool_allocator_allocate(ResourcePoolAllocator *rp, u64 size)
{
	void *pointer = (void *) &rp->memory[rp->allocated_size];
	rp->allocated_size += size;
	if (rp->allocated_size > rp->capacity) {
		Log("Resource pool allocator attempt to allocate %"
				PRIu64 " bytes with only %" PRIu64 " bytes remaining",
				size,
				rp->capacity - rp->allocated_size + size);
		return NULL;
	}

	DLog("Resource pool allocating %" PRIu64 " bytes (%"
			PRIu64 " bytes total)", size, rp->allocated_size);
	return pointer;
}
