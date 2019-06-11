@import AppKit;

#import "os/application.h"

@interface AppDelegate : NSObject<NSApplicationDelegate>

@property(nonatomic, strong) NSApplication *ns_app;

- (instancetype)initWithNSApp:(NSApplication *)ns_app config:(OsApplicationConfig *)config;

@end
