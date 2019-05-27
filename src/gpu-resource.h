#ifndef GpuResource_h
#define GpuResource_h

#include "spacetime.h"
#include "resource-manager.h"

typedef enum {
	GpuResourceStorageModePrivate = 1,
	GpuResourceStorageModeShared = 2,
} GpuResourceStorageMode;

typedef void (*GpuResourceContentInitFn)(void *contents, uintptr_t content_init_options);

typedef struct {
	GpuResourceStorageMode storage_mode;
	GpuResourceContentInitFn content_init_fn;
	uintptr_t content_init_options;
} GpuResourceOptions;

typedef struct {
	GpuResourceOptions resource;
	i64 length;
} GpuBufferOptions;

typedef struct {
	char *source;
} GpuLibraryOptions;

typedef struct {
	char *name;
} GpuFunctionOptions;

void gpu_resource_register_loaders(ResourceManager *rm);

#endif // GpuResource_h
