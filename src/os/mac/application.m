@import AppKit
#import "os/application.h"

@interface AppDelegate : NSObject<NSApplicationDelegate>

- (instancetype)initWithConfig:(OsApplicationConfig *)config;

@end

@implementation AppDelegate {
    OsApplicationNotificationFn _will_terminate;
}

- (instancetype)initWithConfig:(OsApplicationConfig *)config {
    self = [super init];
    if (self) {
        _will_terminate = config->will_terminate;
    }
    return self;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    (void)sender;
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    _will_terminate((__bridge OsNotfication *) notification);
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
    app->application_impl = (void *) CFBridgingRetain(ns_app);

    ns_app.mainMenu = makeMenu();
    ns_app.presentationOptions = NSApplicationPresentationHideMenuBar | NSApplicationPresentationHideDock;

    AppDelegate *delegate = [[AppDelegate alloc] initWithConfig:config];
    [delegate retain];
    [ns_app setDelegate:delegate];

    } // @autoreleasepool
}

void os_application_finish_launching(OsApplication *app)
{
    NSApplication *ns_app = (__bridge NSApplication *) app->application_ipml;
    [ns_app finishLaunching];
}

void os_application_destroy(OsApplication *app)
{
    NSApplication *ns_app = (__bridge NSApplication *) app->application_ipml;
    [ns_app.delegate release];
    CFRelease(app->application_ipml);
}
