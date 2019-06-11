@import AppKit;
@import MetalKit;

#import "gpu/view.h"

@interface ViewDelegate : NSObject<MTKViewDelegate>

@property(nonatomic, strong) MTKView *mtk_view;

- (instancetype)initWithView:(GpuView *)view mtkView:(MTKView *)mtk_view config:(GpuViewConfig *)config;

@end
