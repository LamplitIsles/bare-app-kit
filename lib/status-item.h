#import <assert.h>
#import <js.h>
#import <utf.h>

#import <AppKit/AppKit.h>

#import "bridging.h"
#import "lifecycle.h"

#ifdef BARE_APP_KIT_TESTING
static id bare_app_kit_testing_status_item = nil;
static int bare_app_kit_testing_deleted_references = 0;
#endif

static NSString *
bare_app_kit__string(js_env_t *env, js_value_t *value) {
  int err;
  size_t len;
  err = js_get_value_string_utf8(env, value, NULL, 0, &len);
  assert(err == 0);
  char *buffer = malloc(len + 1);
  size_t written;
  err = js_get_value_string_utf8(env, value, (utf8_t *) buffer, len + 1, &written);
  assert(err == 0);
  NSString *result = [[NSString alloc] initWithBytes:buffer
                                              length:written
                                            encoding:NSUTF8StringEncoding];
  free(buffer);
  return [result autorelease];
}

@interface BareStatusItem : NSObject {
@public
  js_env_t *env;
  js_ref_t *ctx;
  js_ref_t *on_select;
  NSStatusItem *status_item;
  NSMenu *menu;
  NSMutableDictionary *items;
  BOOL observing;
  BOOL destroyed;
  BOOL removed_from_status_bar;
}
- (void)prepareForBareTermination;
- (void)destroy;
@end

@implementation BareStatusItem

- (void)prepareForBareTermination {
  if (observing) {
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:BareApplicationWillTerminateNotification
                                                  object:nil];
    observing = NO;
  }
  for (NSMenuItem *item in [menu itemArray]) item.target = nil;
  if (env == NULL) return;

  int err = js_delete_reference(env, on_select);
  assert(err == 0);
#ifdef BARE_APP_KIT_TESTING
  bare_app_kit_testing_deleted_references++;
#endif
  err = js_delete_reference(env, ctx);
  assert(err == 0);
#ifdef BARE_APP_KIT_TESTING
  bare_app_kit_testing_deleted_references++;
#endif
  env = NULL;
  ctx = NULL;
  on_select = NULL;
}

- (void)bareApplicationWillTerminate:(NSNotification *)notification {
  [self prepareForBareTermination];
}

- (void)destroy {
  if (destroyed) return;
  destroyed = YES;
  [self prepareForBareTermination];
  if (status_item != nil) {
    [[NSStatusBar systemStatusBar] removeStatusItem:status_item];
    removed_from_status_bar = YES;
    [status_item release];
    status_item = nil;
  }
  [menu release];
  menu = nil;
  [items release];
  items = nil;
}

- (void)dealloc {
  [self destroy];
  [super dealloc];
}

- (void)selected:(NSMenuItem *)sender {
  if (destroyed || env == NULL) return;
  NSString *identifier = sender.representedObject;
  if (identifier == nil) return;

  int err;
  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);
  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);
  js_value_t *callback;
  err = js_get_reference_value(env, on_select, &callback);
  assert(err == 0);
  js_value_t *id;
  err = js_create_string_utf8(env,
                              (const utf8_t *) identifier.UTF8String,
                              [identifier lengthOfBytesUsingEncoding:NSUTF8StringEncoding],
                              &id);
  assert(err == 0);
  err = js_call_function(env, receiver, callback, 1, &id, NULL);
  (void) err;
  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

@end

static void
bare_app_kit_status_item_update_native(BareStatusItem *handle,
                                       NSString *identifier,
                                       NSString *title,
                                       BOOL has_title,
                                       BOOL enabled,
                                       BOOL has_enabled) {
  NSMenuItem *item = [handle->items objectForKey:identifier];
  if (has_title) item.title = title;
  if (has_enabled) item.enabled = enabled;
}

static js_value_t *
bare_app_kit_status_item_init(js_env_t *env, js_callback_info_t *info) {
  int err;
  size_t argc = 4;
  js_value_t *argv[4];
  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);
  assert(argc == 4);

  js_value_t *result;
  @autoreleasepool {
    BareStatusItem *handle = [[BareStatusItem alloc] init];
#ifdef BARE_APP_KIT_TESTING
    bare_app_kit_testing_status_item = handle;
    bare_app_kit_testing_deleted_references = 0;
#endif
    NSString *symbol = bare_app_kit__string(env, argv[0]);
    NSString *description = bare_app_kit__string(env, argv[1]);
    handle->status_item = [[[NSStatusBar systemStatusBar]
      statusItemWithLength:NSSquareStatusItemLength] retain];
    handle->menu = [[NSMenu alloc] init];
    handle->menu.autoenablesItems = NO;
    handle->items = [[NSMutableDictionary alloc] init];
    handle->status_item.menu = handle->menu;
    handle->status_item.button.image = [NSImage imageWithSystemSymbolName:symbol
                                                accessibilityDescription:description];
    handle->status_item.button.image.template = YES;
    handle->env = env;
    err = js_create_reference(env, argv[2], 1, &handle->ctx);
    assert(err == 0);
    err = js_create_reference(env, argv[3], 1, &handle->on_select);
    assert(err == 0);
    [[NSNotificationCenter defaultCenter] addObserver:handle
                                             selector:@selector(bareApplicationWillTerminate:)
                                                 name:BareApplicationWillTerminateNotification
                                               object:nil];
    handle->observing = YES;
    err = js_create_external(env, (void *) CFBridgingRetain(handle),
                             bare_app_kit__on_bridged_release, NULL, &result);
    assert(err == 0);
  }
  return result;
}

static BareStatusItem *
bare_app_kit__status_handle(js_env_t *env, js_value_t *value) {
  void *handle;
  int err = js_get_value_external(env, value, &handle);
  assert(err == 0);
  return (__bridge BareStatusItem *) handle;
}

static js_value_t *
bare_app_kit_status_item_add_item(js_env_t *env, js_callback_info_t *info) {
  int err;
  size_t argc = 4;
  js_value_t *argv[4];
  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);
  assert(argc == 4);
  BOOL enabled;
  err = js_get_value_bool(env, argv[3], (bool *) &enabled);
  assert(err == 0);
  @autoreleasepool {
    BareStatusItem *handle = bare_app_kit__status_handle(env, argv[0]);
    NSString *identifier = bare_app_kit__string(env, argv[1]);
    NSMenuItem *item = [[[NSMenuItem alloc] initWithTitle:bare_app_kit__string(env, argv[2])
                                                   action:@selector(selected:)
                                            keyEquivalent:@""] autorelease];
    item.target = handle;
    item.representedObject = identifier;
    item.enabled = enabled;
    [handle->menu addItem:item];
    [handle->items setObject:item forKey:identifier];
  }
  return NULL;
}

static js_value_t *
bare_app_kit_status_item_add_separator(js_env_t *env, js_callback_info_t *info) {
  int err;
  size_t argc = 1;
  js_value_t *argv[1];
  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);
  @autoreleasepool {
    [bare_app_kit__status_handle(env, argv[0])->menu addItem:[NSMenuItem separatorItem]];
  }
  return NULL;
}

static js_value_t *
bare_app_kit_status_item_update_item(js_env_t *env, js_callback_info_t *info) {
  int err;
  size_t argc = 6;
  js_value_t *argv[6];
  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);
  assert(argc == 6);
  bool has_title, enabled, has_enabled;
  err = js_get_value_bool(env, argv[3], &has_title); assert(err == 0);
  err = js_get_value_bool(env, argv[4], &enabled); assert(err == 0);
  err = js_get_value_bool(env, argv[5], &has_enabled); assert(err == 0);
  @autoreleasepool {
    bare_app_kit_status_item_update_native(
      bare_app_kit__status_handle(env, argv[0]),
      bare_app_kit__string(env, argv[1]),
      bare_app_kit__string(env, argv[2]),
      has_title,
      enabled,
      has_enabled);
  }
  return NULL;
}

static js_value_t *
bare_app_kit_status_item_destroy(js_env_t *env, js_callback_info_t *info) {
  int err;
  size_t argc = 1;
  js_value_t *argv[1];
  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);
  [bare_app_kit__status_handle(env, argv[0]) destroy];
  return NULL;
}

#ifdef BARE_APP_KIT_TESTING
static js_value_t *
bare_app_kit_status_item_testing_state(js_env_t *env, js_callback_info_t *info) {
  BareStatusItem *handle = bare_app_kit_testing_status_item;
  int32_t state = 0;
  if (handle != nil) {
    if (handle->env != NULL) state |= 1;
    if (handle->ctx != NULL) state |= 2;
    if (handle->on_select != NULL) state |= 4;
    if (handle->observing) state |= 8;
    for (NSMenuItem *item in [handle->menu itemArray]) {
      if (item.target != nil) {
        state |= 16;
        break;
      }
    }
  }
  js_value_t *result;
  int err = js_create_int32(env, state, &result);
  assert(err == 0);
  return result;
}

static js_value_t *
bare_app_kit_status_item_testing_delete_count(js_env_t *env, js_callback_info_t *info) {
  js_value_t *result;
  int err = js_create_int32(env, bare_app_kit_testing_deleted_references, &result);
  assert(err == 0);
  return result;
}

static js_value_t *
bare_app_kit_status_item_testing_prepare(js_env_t *env, js_callback_info_t *info) {
  [[NSNotificationCenter defaultCenter]
    postNotificationName:BareApplicationWillTerminateNotification
                  object:nil];
  return NULL;
}

static js_value_t *
bare_app_kit_status_item_testing_late_select(js_env_t *env, js_callback_info_t *info) {
  BareStatusItem *handle = bare_app_kit_testing_status_item;
  NSMenuItem *item = [[[NSMenuItem alloc] initWithTitle:@"Open"
                                                 action:nil
                                          keyEquivalent:@""] autorelease];
  item.representedObject = @"open";
  [handle selected:item];
  return NULL;
}
#endif
