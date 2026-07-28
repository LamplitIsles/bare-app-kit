#ifdef NDEBUG
#undef NDEBUG
#endif
#import <assert.h>
#import <AppKit/AppKit.h>

#import "../lib/window.h"
#import "../lib/window-lifecycle.h"

@interface CloseObserver : NSObject {
@public
  int count;
}
@end

@implementation CloseObserver
- (void)closed:(NSNotification *)notification {
  count++;
}
@end

int
main(void) {
  @autoreleasepool {
    [NSApplication sharedApplication];
    NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable;
    CloseObserver *observer = [[CloseObserver alloc] init];

    BareWindow *normal = [[BareWindow alloc]
      initWithContentRect:NSMakeRect(0, 0, 120, 80)
                styleMask:style
                  backing:NSBackingStoreBuffered
                    defer:NO];
    normal.delegate = normal;
    normal.releasedWhenClosed = NO;
    [[NSNotificationCenter defaultCenter] addObserver:observer
                                             selector:@selector(closed:)
                                                 name:NSWindowWillCloseNotification
                                               object:normal];
    [normal performClose:nil];
    assert(observer->count == 1);

    BareWindow *hidden = [[BareWindow alloc]
      initWithContentRect:NSMakeRect(0, 0, 120, 80)
                styleMask:style
                  backing:NSBackingStoreBuffered
                    defer:NO];
    hidden.delegate = hidden;
    hidden.releasedWhenClosed = NO;
    hidden->hides_on_close = YES;
    [[NSNotificationCenter defaultCenter] addObserver:observer
                                             selector:@selector(closed:)
                                                 name:NSWindowWillCloseNotification
                                               object:hidden];
    [hidden orderFront:nil];
    [hidden performClose:nil];
    assert(!hidden.visible);
    assert(observer->count == 1);

    hidden->terminal_close = YES;
    [hidden close];
    assert(observer->count == 2);
    hidden->hides_on_close = NO;

    BareWindow *utility = [[BareWindow alloc]
      initWithContentRect:NSMakeRect(0, 0, 120, 80)
                styleMask:style
                  backing:NSBackingStoreBuffered
                    defer:NO];
    utility.releasedWhenClosed = NO;
    [utility orderOut:nil];

    BareWindow *reopen = [[BareWindow alloc]
      initWithContentRect:NSMakeRect(0, 0, 120, 80)
                styleMask:style
                  backing:NSBackingStoreBuffered
                    defer:NO];
    reopen->hides_on_close = YES;
    reopen.releasedWhenClosed = NO;
    [reopen orderOut:nil];
    BareWindow *otherReopen = [[BareWindow alloc]
      initWithContentRect:NSMakeRect(0, 0, 120, 80)
                styleMask:style
                  backing:NSBackingStoreBuffered
                    defer:NO];
    otherReopen->hides_on_close = YES;
    otherReopen.releasedWhenClosed = NO;
    [otherReopen orderOut:nil];
    BareWindow *expectedReopen = nil;
    for (NSWindow *window in NSApp.orderedWindows) {
      if (window == reopen || window == otherReopen) {
        expectedReopen = (BareWindow *) window;
        break;
      }
    }
    assert(expectedReopen != nil);
    assert(!bare_app_kit_handle_reopen(YES));
    assert(!reopen.visible);
    assert(bare_app_kit_handle_reopen(NO));
    assert(expectedReopen.visible);
    assert((expectedReopen == reopen ? otherReopen : reopen).visible == NO);
    assert(!utility.visible);
    assert(!bare_app_kit_should_terminate_after_last_window_closed(
      @[utility, reopen, otherReopen]));
    assert(bare_app_kit_should_terminate_after_last_window_closed(@[utility]));
    assert(bare_app_kit_should_terminate_after_last_window_closed(@[]));

    [[NSNotificationCenter defaultCenter] removeObserver:observer];
    [normal release];
    [hidden release];
    [utility release];
    [reopen release];
    [otherReopen release];
    [observer release];
  }
  return 0;
}
