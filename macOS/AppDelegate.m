#import "AppDelegate.h"
#import "ViewController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (id)init {
    if (self = [super init]) {
        _window = [[NSWindow alloc] init];
        _window.styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskResizable | NSWindowStyleMaskFullScreen;
        _window.contentViewController = [[ViewController alloc] initWithFrame:[[_window contentView] frame]];
        _window.backgroundColor = [NSColor blackColor];
    }
    return self;
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification {
    [_window makeKeyAndOrderFront:self];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end
