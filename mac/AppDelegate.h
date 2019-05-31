#import "event-queue.h"

@import AppKit;
@import MetalKit;

@interface AppDelegate : NSViewController <NSApplicationDelegate, NSWindowDelegate>

@property (strong, nonatomic) NSWindow *window;

- (instancetype)initWithEventQueue:(EventQueue *)eq;

@end
