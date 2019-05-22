@import MetalKit;

#import "spacetime.h"
#import "MacOSRenderer.h"
#import "MacOSView.h"
#import "renderer.h"
#import "view.h"
#import "world.h"
#import "core.h"
#import "ShaderTypes.h"

const int MACOS_RENDERER_FRAMES_PER_SECOND = 60;

void renderer_initialize(Renderer *r, View *vw, World *w)
{
    MacOSRenderer *macOSRenderer = [[MacOSRenderer alloc]
            initWithRenderer:r view:vw world:w];

    if (!macOSRenderer) {
        r->os_renderer = NULL;
        NSLog(@"Renderer failed initialization");
        return;
    }

    r->os_renderer = (void *) CFBridgingRetain(macOSRenderer);
}

void renderer_terminate(Renderer *r)
{
    CFRelease(r->os_renderer);
}

void renderer_render(Renderer *r, View *vw, World *w)
{
    MacOSRenderer *macOSRenderer = (__bridge MacOSRenderer *)r->os_renderer;
    [macOSRenderer renderWithView:vw world: w];
}

@implementation MacOSRenderer {
    Renderer *_renderer;
    View *_view;
    World *_world;
    id<MTLDevice> _device;
    id<MTLRenderPipelineState> _pipelineState;
    id<MTLCommandQueue> _commandQueue;
    id<MTLBuffer> _vertexBuffer;

    NSUInteger _numVertices;
}

- (instancetype)initWithRenderer:(Renderer *)r view:(View *)vw world:(World *)w {
    self = [super init];
    if (self) {
        _renderer = r;
        _view = vw;
        _world = w;
        MTKView *mtkView = (__bridge MTKView *)vw->os_view;

        [self mtkView:mtkView drawableSizeWillChange:mtkView.drawableSize];
        _device = mtkView.device;

        mtkView.preferredFramesPerSecond = MACOS_RENDERER_FRAMES_PER_SECOND;

        [self loadMetal:mtkView];
    }

    return self;
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

- (void)loadMetal:(MTKView *)mtkView {
    mtkView.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;

    id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];

    id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
    id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];

    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineStateDescriptor.label = @"Simple Pipeline";
    pipelineStateDescriptor.vertexFunction = vertexFunction;
    pipelineStateDescriptor.fragmentFunction = fragmentFunction;
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;

    NSError *error = NULL;
    _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                             error:&error];
    if (!_pipelineState) {
        // Pipeline State creation could fail if we haven't properly set up our pipeline descriptor.
        //  If the Metal API validation is enabled, we can find out more information about what
        //  went wrong.  (Metal API validation is enabled by default when a debug build is run
        //  from Xcode)
        NSLog(@"Failed to created pipeline state, error %@", error);
    }

    NSData *vertexData = [MacOSRenderer generateVertexData];

    // Create a vertex buffer by allocating storage that can be read by the GPU
    _vertexBuffer = [_device newBufferWithLength:vertexData.length
                                         options:MTLResourceStorageModeShared];

    // Copy the vertex data into the vertex buffer by accessing a pointer via
    // the buffer's `contents` property
    memcpy(_vertexBuffer.contents, vertexData.bytes, vertexData.length);

    // Calculate the number of vertices by dividing the byte length by the size of each vertex
    _numVertices = vertexData.length / sizeof(Vertex);

    _commandQueue = [_device newCommandQueue];
}

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size {
    (void)view;
    _view->viewport_size.x = size.width;
    _view->viewport_size.y = size.height;
}

- (void)renderWithView:(View *)vw world:(World *)world {
    //NSLog(@"Render");
    (void)world;
    MTKView *view = (__bridge MTKView *)vw->os_view;
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;

    if (renderPassDescriptor != nil) {
        id<MTLRenderCommandEncoder> renderEncoder =
        [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";

        // Set the region of the drawable to which we'll draw.
        [renderEncoder setViewport:(MTLViewport){0.0, 0.0, _view->viewport_size.x, _view->viewport_size.y, -1.0, 1.0 }];

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

        [renderEncoder setVertexBytes:&_view->viewport_size
                               length:sizeof(_view->viewport_size)
                              atIndex:VertexInputIndexViewportSize];

        // Draw the vertices of the quads
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                          vertexStart:0
                          vertexCount:_numVertices];

        [renderEncoder endEncoding];

        [commandBuffer presentDrawable:view.currentDrawable];
    }

    [commandBuffer commit];
}

- (void)drawInMTKView:(MTKView *)view {
    (void)view;
    @autoreleasepool {
        core_loop_tick(_renderer, _view, _world);
    }
}

@end
