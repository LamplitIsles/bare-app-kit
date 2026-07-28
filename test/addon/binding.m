#import <assert.h>
#import <bare.h>
#import <js.h>

#import "../../lib/status-item.h"

static js_value_t *
bare_app_kit_testing_exports(js_env_t *env, js_value_t *exports) {
  int err;
#define V(name, fn) \
  { \
    js_value_t *value; \
    err = js_create_function(env, name, -1, fn, NULL, &value); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, value); \
    assert(err == 0); \
  }
  V("statusItemInit", bare_app_kit_status_item_init)
  V("statusItemAddItem", bare_app_kit_status_item_add_item)
  V("statusItemDestroy", bare_app_kit_status_item_destroy)
  V("statusItemTestingState", bare_app_kit_status_item_testing_state)
  V("statusItemTestingDeleteCount", bare_app_kit_status_item_testing_delete_count)
  V("statusItemTestingPrepare", bare_app_kit_status_item_testing_prepare)
  V("statusItemTestingLateSelect", bare_app_kit_status_item_testing_late_select)
#undef V
  return exports;
}

BARE_MODULE(bare_app_kit_testing, bare_app_kit_testing_exports)
