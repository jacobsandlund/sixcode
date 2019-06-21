@import AppKit;

#import "Os/Application.h"

@interface AppDelegate : NSObject<NSApplicationDelegate>

@property(nonatomic, strong) NSApplication *ns_app;

- (instancetype)initWithNSApp:(NSApplication *)ns_app;
- (void)registerCallbacks:(OsApplicationCallbacks *)callbacks;

@end
