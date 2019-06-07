#ifndef GpuDevice_h
#define GpuDevice_h

typedef struct {
    void *device_impl;
} GpuDevice;

void gpu_device_init(GpuDevice *device);
void gpu_device_destroy(GpuDevice *device);

#endif // GpuDevice_h
