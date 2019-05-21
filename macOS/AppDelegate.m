#import "AppDelegate.h"
#import "ViewController.h"

@implementation AppDelegate

- (instancetype)init {
    self = [super init];

    if (self) {
        _window = [[NSWindow alloc] init];
        _window.styleMask = NSWindowStyleMaskTitled |
                NSWindowStyleMaskResizable | NSWindowStyleMaskFullScreen;

        NSRect frame = [[_window contentView] frame];
        ViewController *viewController = [[ViewController alloc]
                initWithFrame:frame];

        _window.contentViewController = viewController;
    }

    return self;
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification {
    (void)notification;
    [self.window makeKeyAndOrderFront:self];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    (void)sender;
    return YES;
}

@end
