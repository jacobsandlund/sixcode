#import "AppDelegate.h"

NSMenu *makeMenu() {
    NSMenu *mainMenu = [[NSMenu alloc] init];
    NSMenuItem *mainSpacetimeMenuItem = [[NSMenuItem alloc]
            initWithTitle:@"Spacetime" action:nil keyEquivalent:@""];
    [mainMenu addItem:mainSpacetimeMenuItem];

    NSMenu *mainSpacetimeMenu = [[NSMenu alloc] init];
    mainSpacetimeMenuItem.submenu = mainSpacetimeMenu;

    [mainSpacetimeMenu addItemWithTitle:@"Quit Spacetime"
            action:@selector(terminate:) keyEquivalent:@"q"];

    return mainMenu;
}

int main(int argc, const char *argv[]) {
    (void)argc;
    (void)argv;

    @autoreleasepool {
        [NSApplication sharedApplication];

        AppDelegate *appDelegate = [[AppDelegate alloc] init];
        NSApp.mainMenu = makeMenu();
        NSApp.presentationOptions = NSApplicationPresentationHideMenuBar | NSApplicationPresentationHideDock;
        [NSApp setDelegate:appDelegate];
        [NSApp finishLaunching];

        NSDate *until = [NSDate distantFuture];

        for (;;) {
            NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
                    untilDate:until inMode:NSDefaultRunLoopMode
                    dequeue:YES];

            [NSApp sendEvent: event];
            //NSLog(@"Event: %@", event);
        }
    }

    return 0;
}
