@import AppKit;

#import "os/window.h"

@interface MacWindow : NSWindow
@end

@interface WindowDelegate : NSViewController<NSWindowDelegate>

@property(nonatomic, strong) MacWindow *window;

- (instancetype)initWithMacWindow:(MacWindow *)window;

@end
