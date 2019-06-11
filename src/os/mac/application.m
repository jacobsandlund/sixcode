#import "os/mac/application.h"

@implementation AppDelegate {
    OsApplicationNotificationFn _will_terminate;
}

- (instancetype)initWithNSApp:(NSApplication *)ns_app config:(OsApplicationConfig *)config {
    self = [super init];
    if (self) {
        _ns_app = ns_app;
        _will_terminate = config->will_terminate;
    }
    return self;
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
    NSMenuItem *mainSpacetimeMenuItem = [[NSMenuItem alloc]
            initWithTitle:@"Spacetime" action:nil keyEquivalent:@""];
    [mainMenu addItem:mainSpacetimeMenuItem];

    NSMenu *mainSpacetimeMenu = [[NSMenu alloc] init];
    mainSpacetimeMenuItem.submenu = mainSpacetimeMenu;

    [mainSpacetimeMenu addItemWithTitle:@"Quit Spacetime"
            action:@selector(terminate:) keyEquivalent:@"q"];

    return mainMenu;
}

void os_application_init(OsApplication *app, OsApplicationConfig *config)
{
    @autoreleasepool {

    NSApplication *ns_app = [NSApplication sharedApplication];

    [ns_app setActivationPolicy:NSApplicationActivationPolicyRegular];
    ns_app.presentationOptions = NSApplicationPresentationAutoHideMenuBar | NSApplicationPresentationHideDock;
    [ns_app activateIgnoringOtherApps:YES];
    ns_app.mainMenu = makeMenu();

    AppDelegate *delegate = [[AppDelegate alloc] initWithNSApp:ns_app
            config:config];
    [ns_app setDelegate:delegate];

    app->application_impl = (void *) CFBridgingRetain(delegate);

    } // @autoreleasepool
}

void os_application_destroy(OsApplication *app)
{
    CFRelease(app->application_impl);
}

void os_application_finish_launching(OsApplication *app)
{
    AppDelegate *delegate = (__bridge AppDelegate *)app->application_impl;
    [delegate.ns_app finishLaunching];
}
