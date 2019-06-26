#import "Gpu/PipelineState.h"

#import "Log/Manager.h"

@import MetalKit;

GpuPipelineState *GpuPipelineStateCreate(GpuDevice *device,
                                         GpuPipelineStateConfig *config)
{
    GpuPipelineState *state;

    @autoreleasepool {
        id<MTLDevice> mtl_device = (__bridge id<MTLDevice>)device;
        MTLRenderPipelineDescriptor *pipelineStateDescriptor =
                [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = config->label;
        pipelineStateDescriptor.vertexFunction =
                (__bridge id<MTLFunction>)config->vertex_function;
        pipelineStateDescriptor.fragmentFunction =
                (__bridge id<MTLFunction>)config->fragment_function;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat =
                config->pixel_format;

        NSError *error = NULL;
        id<MTLRenderPipelineState> mtl_pipeline_state = [mtl_device
                newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                               error:&error];

        if (!mtl_pipeline_state) {
            // TODO: log error
            LogError(gLogManager.logs.gpu,
                     "Failed to created pipeline state, error TODO");
            abort();
        }

        state = (__bridge_retained GpuPipelineState *)mtl_pipeline_state;
    }

    return state;
}

void GpuPipelineStateDestroy(GpuPipelineState *state)
{
    @autoreleasepool {
        id<MTLRenderPipelineState> mtl_pipeline_state =
                (__bridge_transfer id<MTLRenderPipelineState>)state;
        mtl_pipeline_state = nil;
    }
}
