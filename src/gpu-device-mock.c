#include <stdlib.h>
#include "gpu-device.h"

#define GpuDeviceMockMaxNumBuffers 64

typedef struct {
	bool allocated;
	bool initialized;
	void *data;
} GpuBufferMock;

typedef struct {
	GpuBufferMock *buffers;
	i64 next_buffer_i;
} GpuDeviceMock;

void gpu_device_mock_init(GpuDevice *device)
{
	GpuDeviceMock *mock = malloc(sizeof *mock);
	mock->buffers = calloc(GpuDeviceMockMaxNumBuffers, sizeof *mock->buffers);
	mock->next_buffer_i = 0;
	device->os_device = (void *) mock;
}

void gpu_device_mock_destroy(GpuDevice *device)
{
	GpuDeviceMock *mock = (GpuDeviceMock *) device->os_device;
	free(mock->buffers);
	free(mock);
}

void gpu_device_init_buffer_with_length(GpuDevice *device, GpuBuffer *buffer, i64 length, GpuBufferOptions options)
{
	GpuDeviceMock *mock_device = (GpuDeviceMock *) device->os_device;
	GpuBufferMock *mock_buffer = mock_device->buffers[mock_device->next_buffer_i++];
	mock_buffer->allocated = true;
	mock_buffer->initialized = true;
	buffer->os_buffer = (void *) mock_buffer;
}

void gpu_device_init_buffer_with_bytes(GpuDevice *device, GpuBuffer *buffer, const void *data, i64 length, GpuBufferOptions options)
{
	gpu_device_init_buffer_with_length(device, buffer, length, options);
	GpuBufferMock *mock_buffer = (GpuBufferMock *) buffer->os_buffer;
	mock_buffer->data = data;
}
