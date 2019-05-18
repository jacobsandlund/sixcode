#import "AppDelegate.h"
#import "ViewController.h"

@implementation AppDelegate

- (instancetype)init {
    self = [super init];
    if (self) {
        _window = [[NSWindow alloc] init];
        _window.styleMask = NSWindowStyleMaskTitled |
                NSWindowStyleMaskResizable | NSWindowStyleMaskFullScreen;
        _window.contentViewController = [[ViewController alloc]
                initWithFrame:[[_window contentView] frame]];
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
