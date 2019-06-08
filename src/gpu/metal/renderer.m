@import AppKit
@import MetalKit

#import "gpu/metal/ShaderTypes.h"

@interface MetalRenderer : NSObject

- (instancetype)initWithDevice:(GpuDevice *)device view:(GpuView *)view;
- (void)renderWithView:(GpuView *)view;

@end

@implementation MetalRenderer {
    id<MTLRenderPipelineState> _pipelineState;
    id<MTLCommandQueue> _commandQueue;
    id<MTLBuffer> _vertexBuffer;
    u64 _numVertices;
}

- (instancetype)initWithDevice:(GpuDevice *)device view:(GpuView *)view;
    MTLDevice *mtl_device = (__bridge MTLDevice *)device->device_impl;
    MTKView *mtk_view = (__bridge MTLDevice *)view->view_impl;
    id<MTLLibrary> defaultLibrary = [mtl_device newDefaultLibrary];

    id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
    id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];

    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineStateDescriptor.label = @"Simple Pipeline";
    pipelineStateDescriptor.vertexFunction = vertexFunction;
    pipelineStateDescriptor.fragmentFunction = fragmentFunction;
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtk_view.colorPixelFormat;

    NSError *error = NULL;
    _pipelineState = [mtl_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
            error:&error];
    if (!_pipelineState) {
        // Pipeline State creation could fail if we haven't properly set up our pipeline descriptor.
        //  If the Metal API validation is enabled, we can find out more information about what
        //  went wrong.  (Metal API validation is enabled by default when a debug build is run
        //  from Xcode)
        log_at_level(&gLogManager.logs.gpu, LogLevelDefault,
                "Failed to created pipeline state, error %@", error);
        return;
    }

    NSData *vertexData = [MetalRenderer generateVertexData];

    // Create a vertex buffer by allocating storage that can be read by the GPU
    _vertexBuffer = [mtl_device newBufferWithLength:vertexData.length
                                         options:MTLResourceStorageModeShared];

    // Copy the vertex data into the vertex buffer by accessing a pointer via
    // the buffer's `contents` property
    memcpy(_vertexBuffer.contents, vertexData.bytes, vertexData.length);

    // Calculate the number of vertices by dividing the byte length by the size of each vertex
    _numVertices = vertexData.length / sizeof(Vertex);

    _commandQueue = [mtl_device newCommandQueue];
}

+ (NSData *)generateVertexData
{
    const Vertex quadVertices[] = {
        // Pixel positions, RGBA colors
        {{-20, 20}, {1, 0, 0, 1}},
        {{20, 20}, {0, 0, 1, 1}},
        {{-20, -20}, {0, 1, 0, 1}},

        {{20, -20}, {1, 0, 0, 1}},
        {{-20, -20}, {0, 1, 0, 1}},
        {{20, 20}, {0, 0, 1, 1}},
    };
    const NSUInteger NUM_COLUMNS = 25;
    const NSUInteger NUM_ROWS = 15;
    const NSUInteger NUM_VERTICES_PER_QUAD = sizeof(quadVertices) / sizeof(Vertex);
    const float QUAD_SPACING = 50.0;

    NSUInteger dataSize = sizeof(quadVertices) * NUM_COLUMNS * NUM_ROWS;
    NSMutableData *vertexData = [[NSMutableData alloc] initWithLength:dataSize];

    Vertex* currentQuad = vertexData.mutableBytes;

    for (NSUInteger row = 0; row < NUM_ROWS; row++) {
        for (NSUInteger column = 0; column < NUM_COLUMNS; column++) {
            float2 upperLeftPosition;
            upperLeftPosition.x = ((-((float)NUM_COLUMNS) / 2.0) + column) * QUAD_SPACING + QUAD_SPACING/2.0;
            upperLeftPosition.y = ((-((float)NUM_ROWS) / 2.0) + row) * QUAD_SPACING + QUAD_SPACING/2.0;

            memcpy(currentQuad, &quadVertices, sizeof(quadVertices));

            for (NSUInteger vertexInQuad = 0; vertexInQuad < NUM_VERTICES_PER_QUAD; vertexInQuad++)
            {
                currentQuad[vertexInQuad].position += upperLeftPosition;
            }

            currentQuad += 6;
        }
    }

    return vertexData;
}

- (void)renderWithView:(GpuView *)view {
    MTKView *mtk_view = (__bridge MTLDevice *)view->view_impl;

    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    MTLRenderPassDescriptor *renderPassDescriptor = mtk_view.currentRenderPassDescriptor;

    if (renderPassDescriptor != nil) {
        id<MTLRenderCommandEncoder> renderEncoder =
        [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";

        MTLViewport viewport = {
            0.0,
            0.0,
            view->viewport_size.x,
            view->viewport_size.y,
            -1.0,
            1.0,
        };
        [renderEncoder setViewport:viewport];

        [renderEncoder setRenderPipelineState:_pipelineState];

        // We call -[MTLRenderCommandEncoder setVertexBuffer:offset:atIndex:] to send data in our
        //   preloaded MTLBuffer from our ObjC code here to our Metal 'vertexShader' function
        // This call has 3 arguments
        //   1) buffer - The buffer object containing the data we want passed down
        //   2) offset - They byte offset from the beginning of the buffer which indicates what
        //      'vertexPointer' point to.  In this case we pass 0 so data at the very beginning is
        //      passed down.
        //      We'll learn about potential uses of the offset in future samples
        //   3) index - An integer index which corresponds to the index of the buffer attribute
        //      qualifier of the argument in our 'vertexShader' function.  Note, this parameter is
        //      the same as the 'index' parameter in
        //              -[MTLRenderCommandEncoder setVertexBytes:length:atIndex:]
        //
        [renderEncoder setVertexBuffer:_vertexBuffer
                                offset:0
                               atIndex:VertexInputIndexVertices];

        [renderEncoder setVertexBytes:&view->viewport_size
                               length:sizeof(view->viewport_size)
                              atIndex:VertexInputIndexViewportSize];

        // Draw the vertices of the quads
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                          vertexStart:0
                          vertexCount:_numVertices];

        [renderEncoder endEncoding];

        [commandBuffer presentDrawable:mtk_view.currentDrawable];
    }

    [commandBuffer commit];
}

@end

void gpu_renderer_init(GpuRenderer *renderer, GpuDevice *device, GpuView *view)
{
    MetalRenderer *mtl_renderer = [[MetalRenderer alloc]
            initWithDevice:device view:view];
    renderer->device = device;
    renderer->renderer_impl = CFBridgingRetain(mtl_renderer);
}

void gpu_renderer_destroy(GpuRenderer *renderer)
{
    CFRelease(renderer->renderer_impl);
}

void gpu_renderer_draw_in_view(GpuRenderer *renderer, GpuView *view)
{
    MetalRenderer *mtl_renderer = (__bridge MetalRenderer *)renderer->renderer_impl;
    [mtl_renderer renderWithView:view];
}
