#ifndef TestAllocator_h
#define TestAllocator_h

#include <stdint.h>

#define TestAllocatorMemorySize 5000000

void test_allocator_reset(void);
void *tmalloc(size_t size);
void tfree(void *mem);

/////////////////////////////////////////////
//
// Implementation
//

static uint8_t tmem[TestAllocatorMemorySize];
static uint8_t *tmem_p = tmem;

void test_allocator_reset(void)
{
    tmem_p = tmem;
}

void *tmalloc(size_t size)
{
	void *p = tmem_p;
	tmem_p += size;
	return p;
}

void tfree(void *mem)
{
    // Memory gets deallocated when test_allocator_reset
    // is called (at end of test)
    (void) mem;
}

#endif // TestAllocator_h
