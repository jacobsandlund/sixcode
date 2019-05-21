@import MetalKit;

#import "View.h"

@interface Renderer : NSObject<MTKViewDelegate>

- (instancetype)initWithView:(View *)view;

@end
