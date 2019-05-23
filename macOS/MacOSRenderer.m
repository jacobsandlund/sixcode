@import MetalKit;

#import "spacetime.h"
#import "MacOSRenderer.h"
#import "ShaderTypes.h"
#import "render-loop.h"

const int MACOS_RENDERER_FRAMES_PER_SECOND = 60;

void renderer_initialize(Renderer *r, MTKView *mtkView, EventQueue *eq, World *w)
{
    MacOSRenderer *macOSRenderer = [[MacOSRenderer alloc]
            initWithRenderer:r mtkView:mtkView eventQueue:eq world:w];

    if (macOSRenderer) {
        r->os_renderer = (void *) CFBridgingRetain(macOSRenderer);
    } else {
        r->os_renderer = NULL;
    }
}

void renderer_terminate(Renderer *r)
{
    CFRelease(r->os_renderer);
}

void renderer_render(Renderer *r, World *w)
{
    MacOSRenderer *macOSRenderer = (__bridge MacOSRenderer *)r->os_renderer;
    [macOSRenderer renderWithWorld:w];
}

@implementation MacOSRenderer {
    Renderer *_renderer;
    EventQueue *_eventQueue;
    World *_world;
    MTKView *_view;
    id<MTLDevice> _device;
    id<MTLRenderPipelineState> _pipelineState;
    id<MTLCommandQueue> _commandQueue;
    id<MTLBuffer> _vertexBuffer;

    float2 _viewportSize;
    u64 _numVertices;
}

- (instancetype)initWithRenderer:(Renderer *)r mtkView:(MTKView *)mtkView eventQueue:(EventQueue *)eq world:(World *)w {
    self = [super init];
    if (self) {
        _renderer = r;
        _eventQueue = eq;
        _world = w;
        _view = mtkView;

        [self mtkView:mtkView drawableSizeWillChange:mtkView.drawableSize];
        _device = mtkView.device;

        mtkView.preferredFramesPerSecond = MACOS_RENDERER_FRAMES_PER_SECOND;

        [self loadMetal];
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

- (void)loadMetal {
    _view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;

    id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];

    id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
    id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];

    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineStateDescriptor.label = @"Simple Pipeline";
    pipelineStateDescriptor.vertexFunction = vertexFunction;
    pipelineStateDescriptor.fragmentFunction = fragmentFunction;
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = _view.colorPixelFormat;

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

- (void)mtkView:(MTKView *)mtkView drawableSizeWillChange:(CGSize)size {
    (void)mtkView;
    _viewportSize = (float2) {
        (float)size.width,
        (float)size.height,
    };
}

- (void)renderWithWorld:(World *)world {
    (void)world;
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    MTLRenderPassDescriptor *renderPassDescriptor = _view.currentRenderPassDescriptor;

    if (renderPassDescriptor != nil) {
        id<MTLRenderCommandEncoder> renderEncoder =
        [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";

        MTLViewport viewport = {
            0.0,
            0.0,
            _viewportSize.x,
            _viewportSize.y,
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

        [renderEncoder setVertexBytes:&_viewportSize
                               length:sizeof(_viewportSize)
                              atIndex:VertexInputIndexViewportSize];

        // Draw the vertices of the quads
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                          vertexStart:0
                          vertexCount:_numVertices];

        [renderEncoder endEncoding];

        [commandBuffer presentDrawable:_view.currentDrawable];
    }

    [commandBuffer commit];
}

- (void)drawInMTKView:(MTKView *)mtkView {
    _view = mtkView;
    @autoreleasepool {
        render_loop_one_iteration(_renderer, _eventQueue, _world);
    }
}

@end
