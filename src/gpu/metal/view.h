@import AppKit;
@import MetalKit;

#import "gpu/view.h"

@interface ViewDelegate : NSObject<MTKViewDelegate>

@property(nonatomic, strong) MTKView *mtk_view;
@property(nonatomic, readonly) float2 viewport_size;

- (instancetype)initWithMtkView:(MTKView *)mtk_view config:(GpuViewConfig *)config;

@end
