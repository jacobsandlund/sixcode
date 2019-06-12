#import "gpu/metal/view.h"
#import "log/manager.h"

@implementation ViewDelegate {
    GpuViewFn _draw_in_view;
    GpuViewSizeChangedFn _size_changed;
}

- (instancetype)initWithMtkView:(MTKView *)mtk_view config:(GpuViewConfig *)config {
    self = [super init];
    if (self) {
        _size_changed = config->size_changed;
        _draw_in_view = config->draw_in_view;
        _mtk_view = mtk_view;

        [self mtkView:mtk_view drawableSizeWillChange:mtk_view.drawableSize];
    }

    return self;
}

- (void)mtkView:(MTKView *)mtk_view drawableSizeWillChange:(CGSize)size {
    _mtk_view = mtk_view;
    _viewport_size = (float2) {
        (float)size.width,
        (float)size.height,
    };
    LogDebug(gLogManager.logs.os, "drawableSizeWillChange %g, %g", _viewport_size.x, _viewport_size.y);

    _size_changed((__bridge GpuView *)self, _viewport_size);
}

- (void)drawInMTKView:(MTKView *)mtk_view {
    _mtk_view = mtk_view;
    _draw_in_view((__bridge GpuView *)self);
}

@end

GpuView *gpu_view_create(GpuDevice *device, OsScreenFrame visible_frame, GpuViewConfig *config)
{
    GpuView *view;

    @autoreleasepool {

    id<MTLDevice> mtl_device = (__bridge id<MTLDevice>)device;

    NSRect frame = NSMakeRect(
            visible_frame.origin.x,
            visible_frame.origin.y,
            visible_frame.size.x,
            visible_frame.size.y);

    MTKView *mtk_view = [[MTKView alloc] initWithFrame: frame
            device: mtl_device];
    mtk_view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    mtk_view.preferredFramesPerSecond = config->preferred_frames_per_second;

    ViewDelegate *delegate = [[ViewDelegate alloc]
            initWithMtkView:mtk_view config:config];
    mtk_view.delegate = delegate;

    view = (__bridge_retained GpuView *)delegate;

    } // @autoreleasepool

    return view;
}

void gpu_view_destroy(GpuView *view)
{
    @autoreleasepool {
        ViewDelegate *delegate = (__bridge_transfer ViewDelegate *)view;
        delegate = nil;
    }
}

float2 gpu_view_viewport_size(GpuView *view)
{
    ViewDelegate *delegate = (__bridge ViewDelegate *)view;
    return delegate.viewport_size;
}
