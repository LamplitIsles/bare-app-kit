#import <assert.h>
#import <bare.h>
#import <js.h>

#import <AppKit/AppKit.h>

#import "bridging.h"

@interface BareWindow : NSWindow <NSWindowDelegate> {
@public
  js_env_t *env;
  js_ref_t *ctx;
  js_ref_t *on_did_resize;
  js_ref_t *on_did_move;
  js_ref_t *on_will_close;
  BOOL hides_on_close;
  BOOL terminal_close;
}

@end

@implementation BareWindow

- (BOOL)bareHidesOnClose {
  return hides_on_close;
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
  if (!hides_on_close || terminal_close) return YES;
  [self orderOut:sender];
  return NO;
}

- (void)prepareForBareTermination {
  if (env == NULL) return;

  int err;

  self.delegate = nil;

  err = js_delete_reference(env, on_did_resize);
  assert(err == 0);

  err = js_delete_reference(env, on_did_move);
  assert(err == 0);

  err = js_delete_reference(env, on_will_close);
  assert(err == 0);

  err = js_delete_reference(env, ctx);
  assert(err == 0);

  env = NULL;
  ctx = NULL;
  on_did_resize = NULL;
  on_did_move = NULL;
  on_will_close = NULL;
}

- (void)dealloc {
  [self prepareForBareTermination];

  [super dealloc];
}

- (void)windowDidResize:(NSNotification *)notification {
  if (env == NULL) return;

  int err;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);

  js_value_t *callback;
  err = js_get_reference_value(env, on_did_resize, &callback);
  assert(err == 0);

  err = js_call_function(env, receiver, callback, 0, NULL, NULL);
  (void) err;

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

- (void)windowDidMove:(NSNotification *)notification {
  if (env == NULL) return;

  int err;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);

  js_value_t *callback;
  err = js_get_reference_value(env, on_did_move, &callback);
  assert(err == 0);

  err = js_call_function(env, receiver, callback, 0, NULL, NULL);
  (void) err;

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

- (void)windowWillClose:(NSNotification *)notification {
  if (env == NULL) return;

  int err;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);

  js_value_t *callback;
  err = js_get_reference_value(env, on_will_close, &callback);
  assert(err == 0);

  err = js_call_function(env, receiver, callback, 0, NULL, NULL);
  (void) err;

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

@end

static js_value_t *
bare_app_kit_window_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 10;
  js_value_t *argv[10];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 10);

  double x;
  err = js_get_value_double(env, argv[0], &x);
  assert(err == 0);

  double y;
  err = js_get_value_double(env, argv[1], &y);
  assert(err == 0);

  double width;
  err = js_get_value_double(env, argv[2], &width);
  assert(err == 0);

  double height;
  err = js_get_value_double(env, argv[3], &height);
  assert(err == 0);

  int32_t style_mask;
  err = js_get_value_int32(env, argv[4], &style_mask);
  assert(err == 0);

  bool defer;
  err = js_get_value_bool(env, argv[5], &defer);
  assert(err == 0);

  js_value_t *result;

  @autoreleasepool {
    BareWindow *handle = [[BareWindow alloc]
      initWithContentRect:NSMakeRect(x, y, width, height)
                styleMask:style_mask
                  backing:NSBackingStoreBuffered
                    defer:defer];

    handle.delegate = handle;

    err = js_create_external(env, (void *) CFBridgingRetain(handle), bare_app_kit__on_bridged_release, NULL, &result);
    assert(err == 0);

    handle->env = env;

    err = js_create_reference(env, argv[6], 1, &handle->ctx);
    assert(err == 0);

    err = js_create_reference(env, argv[7], 1, &handle->on_did_resize);
    assert(err == 0);

    err = js_create_reference(env, argv[8], 1, &handle->on_did_move);
    assert(err == 0);

    err = js_create_reference(env, argv[9], 1, &handle->on_will_close);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_app_kit_window_content_view(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  js_value_t *result = NULL;

  @autoreleasepool {
    BareWindow *window = (__bridge BareWindow *) handle;

    if (argc == 1) {
      NSView *content_view = window.contentView;

      err = js_create_external(env, (void *) CFBridgingRetain(content_view), bare_app_kit__on_bridged_release, NULL, &result);
      assert(err == 0);
    } else {
      void *handle;
      err = js_get_value_external(env, argv[1], &handle);
      assert(err == 0);

      NSView *content_view = (__bridge NSView *) handle;

      window.contentView = content_view;
    }
  }

  return result;
}

static js_value_t *
bare_app_kit_window_title(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  js_value_t *result = NULL;

  @autoreleasepool {
    BareWindow *window = (__bridge BareWindow *) handle;

    if (argc == 1) {
      NSString *title = window.title;

      err = js_create_string_utf8(env, (const utf8_t *) [title UTF8String], -1, &result);
      assert(err == 0);
    } else {
      size_t len;
      err = js_get_value_string_utf8(env, argv[1], NULL, 0, &len);
      assert(err == 0);

      len += 1 /* NULL */;
      char *title = malloc(len);

      err = js_get_value_string_utf8(env, argv[1], (utf8_t *) title, len, &len);
      assert(err == 0);

      window.title = [NSString stringWithUTF8String:title];
      free(title);
    }
  }

  return result;
}

static js_value_t *
bare_app_kit_window_titlebar_appears_transparent(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  js_value_t *result = NULL;

  @autoreleasepool {
    BareWindow *window = (__bridge BareWindow *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, window.titlebarAppearsTransparent, &result);
      assert(err == 0);
    } else {
      bool value;
      err = js_get_value_bool(env, argv[1], &value);
      assert(err == 0);

      window.titlebarAppearsTransparent = value;
    }
  }

  return result;
}

static js_value_t *
bare_app_kit_window_center(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  @autoreleasepool {
    BareWindow *window = (__bridge BareWindow *) handle;

    [window center];
  }

  return NULL;
}

static js_value_t *
bare_app_kit_window_close(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  @autoreleasepool {
    BareWindow *window = (__bridge BareWindow *) handle;

    window->terminal_close = YES;
    [window close];
    window->terminal_close = NO;
  }

  return NULL;
}

static js_value_t *
bare_app_kit_window_hides_on_close(js_env_t *env, js_callback_info_t *info) {
  int err;
  size_t argc = 2;
  js_value_t *argv[2];
  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);
  assert(argc == 1 || argc == 2);
  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);
  BareWindow *window = (__bridge BareWindow *) handle;
  if (argc == 1) {
    js_value_t *result;
    err = js_get_boolean(env, window->hides_on_close, &result);
    assert(err == 0);
    return result;
  }
  bool value;
  err = js_get_value_bool(env, argv[1], &value);
  assert(err == 0);
  window->hides_on_close = value;
  return NULL;
}

static js_value_t *
bare_app_kit_window_hide(js_env_t *env, js_callback_info_t *info) {
  int err;
  size_t argc = 1;
  js_value_t *argv[1];
  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);
  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);
  [(__bridge BareWindow *) handle orderOut:nil];
  return NULL;
}

static js_value_t *
bare_app_kit_window_show(js_env_t *env, js_callback_info_t *info) {
  int err;
  size_t argc = 1;
  js_value_t *argv[1];
  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);
  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);
  [NSApp activateIgnoringOtherApps:YES];
  [(__bridge BareWindow *) handle makeKeyAndOrderFront:nil];
  return NULL;
}

static js_value_t *
bare_app_kit_window_make_key_window(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  @autoreleasepool {
    BareWindow *window = (__bridge BareWindow *) handle;

    [window makeKeyWindow];
  }

  return NULL;
}

static js_value_t *
bare_app_kit_window_order_back(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  @autoreleasepool {
    BareWindow *window = (__bridge BareWindow *) handle;

    [window orderBack:window];
  }

  return NULL;
}

static js_value_t *
bare_app_kit_window_order_front(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  @autoreleasepool {
    BareWindow *window = (__bridge BareWindow *) handle;

    [window orderFront:window];
  }

  return NULL;
}
