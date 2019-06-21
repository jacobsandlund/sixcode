#import "Os/Mac/Window.h"
#import "Gpu/Metal/View.h"

// Look at this for a lot of the boilerplate here:
//   https://hero.handmade.network/forums/code-discussion/t/1409-main_game_loop_on_os_x

@implementation MacWindow

- (BOOL)acceptsFirstResponder { return YES; }
- (BOOL)canBecomeKeyWindow { return YES; }
- (BOOL)canBecomeMainWindow { return YES; }

@end

@implementation WindowDelegate

- (instancetype)initWithMacWindow:(MacWindow *)window {
    self = [super init];
    if (self) {
        _window = window;
    }
    return self;
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    (void)sender;
    return YES;
}

- (void)windowWillClose:(NSNotification *)notification {
    MacWindow *window = notification.object;
    if (window.isMainWindow) {
        [NSApp terminate:nil];
    }
}

- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize {
    (void)sender;
    return frameSize;
}

@end

OsWindow *OsWindowCreate(GpuView *view)
{
    OsWindow *window;

    @autoreleasepool {

    MacWindow *mac_window = [[MacWindow alloc] init];
    mac_window.styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;

    ViewDelegate *view_delegate = (__bridge ViewDelegate *)view;
    WindowDelegate *delegate = [[WindowDelegate alloc]
            initWithMacWindow:mac_window];

    delegate.view = view_delegate.mtk_view;
    mac_window.delegate = delegate;
    mac_window.contentViewController = delegate;

    window = (__bridge_retained OsWindow *)delegate;

    } // @autoreleasepool

    return window;
}

void OsWindowDestroy(OsWindow *window)
{
    @autoreleasepool {
        WindowDelegate *delegate = (__bridge_transfer WindowDelegate *)window;
        delegate = nil;
    }
}

void OsWindowShow(OsWindow *window)
{
    @autoreleasepool {
        WindowDelegate *delegate = (__bridge WindowDelegate *) window;
        MacWindow *mac_window = delegate.window;
        [mac_window makeKeyAndOrderFront:delegate];
        [mac_window toggleFullScreen:delegate];
    }
}
