#ifndef GpuResource_h
#define GpuResource_h

#include "spacetime.h"

typedef enum {
	GpuResourceOptionsStoragePrivate = 1,
	GpuResourceOptionsStorageShared = 2,
} GpuResourceOptionsStorage;

typedef void (*GpuResourceContentInitFn)(void *contents, uintptr_t content_init_options);

typedef struct {
	GpuResourceOptionsStorage storage_options;
	GpuResourceContentInitFn content_init_fn;
	uintptr_t content_init_options;
} GpuResourceOptions;

typedef struct {
	char *label;
	GpuResourceOptions options;
} GpuResource;

typedef struct {
	GpuResource resource;
	void *os_buffer;
} GpuBuffer;

void gpu_resource_init(GpuResource *resource, GpuResourceOptions options);
void gpu_buffer_label(GpuBuffer *buffer, const char *label);

#endif // GpuResource_h
