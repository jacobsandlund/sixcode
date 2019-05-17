#import "AppDelegate.h"

NSMenu *makeMenu() {
    NSMenu *mainMenu = [[NSMenu alloc] init];
    NSMenuItem *mainSixcodeMenuItem = [[NSMenuItem alloc] initWithTitle:@"Sixcode"
                                                                 action:nil
                                                          keyEquivalent:@""];
    [mainMenu addItem:mainSixcodeMenuItem];
    
    NSMenu *mainSixcodeMenu = [[NSMenu alloc] init];
    mainSixcodeMenuItem.submenu = mainSixcodeMenu;
    
    [mainSixcodeMenu addItemWithTitle:@"Quit Sixcode"
                               action:@selector(terminate:)
                        keyEquivalent:@"q"];
    
    return mainMenu;
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        [NSApplication sharedApplication];

        AppDelegate *appDelegate = [[AppDelegate alloc] init];
        NSApp.mainMenu = makeMenu();
        NSApp.presentationOptions = NSApplicationPresentationHideMenuBar | NSApplicationPresentationHideDock;
        [NSApp setDelegate:appDelegate];
        [NSApp run];
    }
    
    return 0;
}
