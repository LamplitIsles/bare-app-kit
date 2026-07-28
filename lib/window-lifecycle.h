#import <AppKit/AppKit.h>

static BOOL
bare_app_kit_handle_reopen(BOOL has_visible_windows) {
  if (has_visible_windows) return NO;

  SEL hides_on_close = @selector(bareHidesOnClose);
  for (NSWindow *window in NSApp.orderedWindows) {
    if (![window respondsToSelector:hides_on_close]) continue;
    BOOL (*get_hides_on_close)(id, SEL) =
      (BOOL (*)(id, SEL)) [window methodForSelector:hides_on_close];
    if (!get_hides_on_close(window, hides_on_close)) continue;
    [NSApp activateIgnoringOtherApps:YES];
    [window makeKeyAndOrderFront:nil];
    return YES;
  }
  return NO;
}
