#ifndef ResourcePointerAllocator_h
#define ResourcePointerAllocator_h

#include "spacetime.h"

typedef struct {
	u8 *memory;
	u64 capacity;
	u64 allocated;
} ResourcePointerAllocator;

void resource_pointer_allocator_init(ResourcePointerAllocator *pa, u64 capacity);
void resource_pointer_allocator_destroy(ResourcePointerAllocator *pa);
void *resource_pointer_allocator_alloc(ResourcePointerAllocator *pa, u64 size);
void *resource_pointer_allocator_top(ResourcePointerAllocator *pa);
void resource_pointer_allocator_reset_top(ResourcePointerAllocator *pa, void *top);

#endif // ResourcePointerAllocator_h
