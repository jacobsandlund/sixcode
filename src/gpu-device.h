#ifndef GpuDevice_h
#define GpuDevice_h

#include "spacetime.h"
#include "gpu-resource.h"

typedef struct {
	void *os_device;
} GpuDevice;

void gpu_device_init_buffer_with_length(GpuDevice *device, GpuBuffer *buffer, i64 length, GpuBufferOptions options);
void gpu_device_init_buffer_with_bytes(GpuDevice *device, GpuBuffer *buffer, const void *data, i64 length, GpuBufferOptions options);

#endif // GpuDevice_h
