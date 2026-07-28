#import <AppKit/AppKit.h>

static BOOL
bare_app_kit_native_window_hides_on_close(NSWindow *window) {
  SEL selector = @selector(bareHidesOnClose);
  if (![window respondsToSelector:selector]) return NO;
  BOOL (*get_hides_on_close)(id, SEL) =
    (BOOL (*)(id, SEL)) [window methodForSelector:selector];
  return get_hides_on_close(window, selector);
}

static BOOL
bare_app_kit_should_terminate_after_last_window_closed(
  NSArray<NSWindow *> *windows) {
  for (NSWindow *window in windows) {
    if (bare_app_kit_native_window_hides_on_close(window)) return NO;
  }
  return YES;
}

static BOOL
bare_app_kit_handle_reopen(BOOL has_visible_windows) {
  if (has_visible_windows) return NO;

  for (NSWindow *window in NSApp.orderedWindows) {
    if (!bare_app_kit_native_window_hides_on_close(window)) continue;
    [NSApp activateIgnoringOtherApps:YES];
    [window makeKeyAndOrderFront:nil];
    return YES;
  }
  return NO;
}
