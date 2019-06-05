#import "AppDelegate.h"
#import "MacOSRenderer.h"
#import "world.h"
#import <stdlib.h>

@implementation AppDelegate {
    EventQueue *_eventQueue;
    World *_world;
    Renderer *_renderer;
}

- (instancetype)initWithEventQueue:(EventQueue *)eq {
    self = [super init];

    if (self) {
        _window = [[NSWindow alloc] init];
        _window.styleMask = NSWindowStyleMaskTitled |
                NSWindowStyleMaskResizable | NSWindowStyleMaskFullScreen;

        _eventQueue = eq;
        _world = malloc(sizeof *_world);
        _renderer = malloc(sizeof *_renderer);

        world_initialize(_world);

        NSSize size = [_window contentView].frame.size;
        _world->viewport.size = (float2) {
            size.width,
            size.height,
        };

        world_load(_world);

        _window.contentViewController = self;
    }

    return self;
}

- (void)loadView {
    NSRect frame = NSMakeRect(0.0, 0.0,
            _world->viewport.size.x, _world->viewport.size.y);
    MTKView *mtkView = [[MTKView alloc] initWithFrame: frame
            device: MTLCreateSystemDefaultDevice()];

    if (!mtkView.device) {
        Log("Metal is not supported on this device");
        return;
    }

    renderer_initialize(_renderer, mtkView, _eventQueue, _world);
    mtkView.delegate = (__bridge MacOSRenderer *) _renderer->os_renderer;

    if (!mtkView.delegate) {
        Log("Renderer failed initialization");
        return;
    }

    self.view = mtkView;
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification {
    (void)notification;
    [_window makeKeyAndOrderFront:self];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    (void)sender;
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    (void)notification;
    world_terminate(_world);
    renderer_terminate(_renderer);

    free(_world);
    free(_renderer);
}

@end
