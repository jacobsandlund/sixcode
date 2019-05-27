#include "gpu-resource.h"
#include "resource.h"

typedef struct {
	u8 *contents;
	GpuResourceStorageMode storage_mode;
} GpuBufferMock;

static void *gpu_resource_buffer_init(uintptr_t raw_options)
{
	GpuBufferOptions *options = (GpuBufferOptions *) raw_options;
	GpuBufferMock *buffer = malloc(sizeof *buffer);
	buffer->contents = malloc(options->length);
	buffer->storage_mode = options->resource.storage_mode;
	if (options->resource.content_init_fn) {
		options->resource.content_init_fn(
				(void *) buffer->contents,
				options->resource.content_init_options);
	}
}

static void gpu_resource_buffer_destroy(void *pointer)
{
	GpuBufferMock *buffer = (GpuBufferMock *) pointer;
	free(buffer->contents);
	free(buffer);
}

void gpu_resource_register_loaders(ResourceManager *rm)
{
	ResourceLoader buffer_loader = {
		.type = GpuResourceTypeBuffer,
		.init = gpu_resource_buffer_init,
		.destroy = gpu_resource_buffer_destroy,
	};

	resource_manager_register_loader(rm, &buffer_loader);
}
