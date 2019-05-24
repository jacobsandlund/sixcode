#include "gpu-resource.h"

void gpu_resource_init(GpuResource *resource, GpuResourceOptions options)
{
	resource->label = NULL;
	resource->options = options;
}

void gpu_buffer_label(GpuBuffer *buffer, const char *label)
{
	buffer->resource.label = label;
}
