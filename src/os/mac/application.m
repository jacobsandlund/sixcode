#import "os/mac/application.h"
#import "log/manager.h"

static void os_application_early_will_terminate(void)
{
    LogDefault(gLogManager.logs.os, "Application will_terminate called before callbacks registered");
}

@implementation AppDelegate {
    OsApplicationNotificationFn _will_terminate;
}

- (instancetype)initWithNSApp:(NSApplication *)ns_app {
    self = [super init];
    if (self) {
        _ns_app = ns_app;
        _will_terminate = os_application_early_will_terminate;
    }
    return self;
}

- (void)registerCallbacks:(OsApplicationCallbacks *)callbacks {
    _will_terminate = callbacks->will_terminate;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    (void)sender;
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    (void)notification;
    _will_terminate();
}

@end

NSMenu *makeMenu() {
    NSMenu *mainMenu = [[NSMenu alloc] init];

    {
        NSMenuItem *spacetimeMenuItem = [[NSMenuItem alloc]
                initWithTitle:@"Spacetime" action:nil keyEquivalent:@""];
        [mainMenu addItem:spacetimeMenuItem];

        NSMenu *spacetimeMenu = [[NSMenu alloc] init];
        spacetimeMenuItem.submenu = spacetimeMenu;

        [spacetimeMenu addItemWithTitle:@"Quit Spacetime"
                action:@selector(terminate:) keyEquivalent:@"q"];
    }

    {
        NSMenuItem *viewMenuItem = [[NSMenuItem alloc]
                initWithTitle:@"View" action:nil keyEquivalent:@""];
        [mainMenu addItem:viewMenuItem];

        NSMenu *viewMenu = [[NSMenu alloc] initWithTitle:@"View"];
        viewMenuItem.submenu = viewMenu;

        [viewMenu addItemWithTitle:@"Toggle Fullscreen"
                action:@selector(toggleFullScreen:) keyEquivalent:@"f"];
    }

    return mainMenu;
}

OsApplication *OsApplicationCreate(void)
{
    OsApplication *app;

    @autoreleasepool {

    NSApplication *ns_app = [NSApplication sharedApplication];

    [ns_app setActivationPolicy:NSApplicationActivationPolicyRegular];
    ns_app.presentationOptions = NSApplicationPresentationDefault;
    [ns_app activateIgnoringOtherApps:YES];

    ns_app.mainMenu = makeMenu();

    AppDelegate *delegate = [[AppDelegate alloc] initWithNSApp:ns_app];
    [ns_app setDelegate:delegate];

    app = (__bridge_retained OsApplication *) delegate;

    } // @autoreleasepool

    return app;
}

void OsApplicationDestroy(OsApplication *app)
{
    @autoreleasepool {
        AppDelegate *delegate = (__bridge_transfer AppDelegate *)app;
        delegate = nil;
    }
}

void OsApplicationRegisterCallbacks(OsApplication *app, OsApplicationCallbacks *callbacks)
{
    AppDelegate *delegate = (__bridge AppDelegate *)app;
    [delegate registerCallbacks:callbacks];
}

void OsApplicationFinishLaunching(OsApplication *app)
{
    @autoreleasepool {
        AppDelegate *delegate = (__bridge AppDelegate *)app;
        [delegate.ns_app finishLaunching];
    }
}
