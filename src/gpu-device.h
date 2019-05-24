#ifndef GpuDevice_h
#define GpuDevice_h

typedef struct {
	void *os_buffer;
} GpuBuffer;

typedef enum {
} GpuBufferOptions;

void gpu_device_new_buffer_with_bytes(const void *data, i64 length, GpuBufferOptions options);

#endif // GpuDevice_h
