@import MetalKit;

@interface Renderer : NSObject<MTKViewDelegate>

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;

@end
