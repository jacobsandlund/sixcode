#import "ViewController.h"
#import "Renderer.h"

@implementation ViewController {
    NSRect _frame;
    MTKView *_view;

    Renderer *_renderer;
}

- (instancetype)initWithFrame:(NSRect)frame {
    self = [super init];
    if (self) {
        _frame = frame;
    }
    return self;
}

- (void)loadView {
    _view = [[MTKView alloc] initWithFrame:_frame
            device:MTLCreateSystemDefaultDevice()];

    if (!_view.device) {
        NSLog(@"Metal is not supported on this device");
        return;
    }

    _renderer = [[Renderer alloc] initWithMetalKitView:_view];

    if (!_renderer) {
        NSLog(@"Renderer failed initialization");
        return;
    }

    [_renderer mtkView:_view drawableSizeWillChange:_view.drawableSize];

    _view.delegate = _renderer;
    self.view = _view;
}

@end
