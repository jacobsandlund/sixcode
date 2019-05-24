#ifndef ResourcePoolAllocator_h
#define ResourcePoolAllocator_h

#include "spacetime.h"

typedef struct {
	u8 *memory;
	u64 capacity;
	u64 allocated_size;
} ResourcePoolAllocator;

void resource_pool_allocator_init(ResourcePoolAllocator *rp, u64 capacity);
void resource_pool_allocator_destroy(ResourcePoolAllocator *rp);
void *resource_pool_allocator_allocate(ResourcePoolAllocator *rp, u64 size);

#endif // ResourcePoolAllocator_h
