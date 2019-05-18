@import MetalKit;

@interface Renderer : NSObject<MTKViewDelegate>

- (instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;

@end
