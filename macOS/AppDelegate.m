#import "AppDelegate.h"
#import "MacOSRenderer.h"
#import "MacOSView.h"
#import "world.h"
#import <stdlib.h>

@implementation AppDelegate {
    World *_world;
    View *_view;
    Renderer *_renderer;
}

- (instancetype)init {
    self = [super init];

    if (self) {
        NSLog(@"AppDelegate init");
        _window = [[NSWindow alloc] init];
        _window.styleMask = NSWindowStyleMaskTitled |
                NSWindowStyleMaskResizable | NSWindowStyleMaskFullScreen;
        _world = malloc(sizeof *_world);
        _view = malloc(sizeof *_view);
        _renderer = malloc(sizeof *_renderer);

        NSSize size = [_window contentView].frame.size;
        _view->viewport_size = (float2) {
            size.width,
            size.height,
        };

        world_initialize(_world);
        world_load(_world);

        _window.contentViewController = self;
    }

    return self;
}

- (void)loadView {
    NSLog(@"Load view");
    view_initialize(_view, _view->viewport_size);
    renderer_initialize(_renderer, _view, _world);

    MacOSView *macOSView = (__bridge MacOSView *) _view->os_view;
    macOSView.delegate = (__bridge MacOSRenderer *) _renderer->os_renderer;
    self.view = macOSView;
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification {
    (void)notification;
    NSLog(@"AppDelegate applicationWillFinishLaunching");
    [self.window makeKeyAndOrderFront:self];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    (void)sender;
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    (void)notification;
    world_terminate(_world);
    view_terminate(_view);
    renderer_terminate(_renderer);

    free(_world);
    free(_view);
    free(_renderer);
}

@end
