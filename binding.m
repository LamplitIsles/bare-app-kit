#import <assert.h>
#import <bare.h>
#import <js.h>
#import <utf.h>

#import <AppKit/AppKit.h>

#import "lib/button.h"
#import "lib/control.h"
#import "lib/slider.h"
#import "lib/switch.h"
#import "lib/status-item.h"
#import "lib/text.h"
#import "lib/text-field.h"
#import "lib/window.h"

static js_value_t *
bare_app_kit_exports(js_env_t *env, js_value_t *exports) {
  int err;

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, NULL, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("buttonInit", bare_app_kit_button_init)
  V("buttonTitle", bare_app_kit_button_title)
  V("buttonAlternateTitle", bare_app_kit_button_alternate_title)
  V("buttonState", bare_app_kit_button_state)
  V("buttonAllowsMixedState", bare_app_kit_button_allows_mixed_state)
  V("buttonBezelStyle", bare_app_kit_button_bezel_style)
  V("buttonBordered", bare_app_kit_button_bordered)
  V("buttonTransparent", bare_app_kit_button_transparent)
  V("buttonShowsBorderOnlyWhileMouseInside", bare_app_kit_button_shows_border_only_while_mouse_inside)
  V("buttonSpringLoaded", bare_app_kit_button_spring_loaded)
  V("buttonHasDestructiveAction", bare_app_kit_button_has_destructive_action)
  V("buttonKeyEquivalent", bare_app_kit_button_key_equivalent)
  V("buttonKeyEquivalentModifierMask", bare_app_kit_button_key_equivalent_modifier_mask)
  V("buttonSetButtonType", bare_app_kit_button_set_button_type)
  V("buttonSetNextState", bare_app_kit_button_set_next_state)
  V("buttonHighlight", bare_app_kit_button_highlight)
  V("buttonSetPeriodicDelay", bare_app_kit_button_set_periodic_delay)
  V("buttonGetPeriodicDelay", bare_app_kit_button_get_periodic_delay)

  V("controlEnabled", bare_app_kit_control_enabled)
  V("controlContinuous", bare_app_kit_control_continuous)
  V("controlIgnoresMultiClick", bare_app_kit_control_ignores_multi_click)
  V("controlHighlighted", bare_app_kit_control_highlighted)
  V("controlRefusesFirstResponder", bare_app_kit_control_refuses_first_responder)
  V("controlTag", bare_app_kit_control_tag)
  V("controlSize", bare_app_kit_control_size)
  V("controlStringValue", bare_app_kit_control_string_value)
  V("controlIntValue", bare_app_kit_control_int_value)
  V("controlIntegerValue", bare_app_kit_control_integer_value)
  V("controlFloatValue", bare_app_kit_control_float_value)
  V("controlDoubleValue", bare_app_kit_control_double_value)
  V("controlSizeToFit", bare_app_kit_control_size_to_fit)
  V("controlPerformClick", bare_app_kit_control_perform_click)

  V("sliderInit", bare_app_kit_slider_init)
  V("sliderMinValue", bare_app_kit_slider_min_value)
  V("sliderMaxValue", bare_app_kit_slider_max_value)
  V("sliderAltIncrementValue", bare_app_kit_slider_alt_increment_value)
  V("sliderKnobThickness", bare_app_kit_slider_knob_thickness)
  V("sliderVertical", bare_app_kit_slider_vertical)
  V("sliderType", bare_app_kit_slider_type)
  V("sliderNumberOfTickMarks", bare_app_kit_slider_number_of_tick_marks)
  V("sliderTickMarkPosition", bare_app_kit_slider_tick_mark_position)
  V("sliderAllowsTickMarkValuesOnly", bare_app_kit_slider_allows_tick_mark_values_only)
  V("sliderTickMarkValueAtIndex", bare_app_kit_slider_tick_mark_value_at_index)
  V("sliderClosestTickMarkValueToValue", bare_app_kit_slider_closest_tick_mark_value_to_value)

  V("switchInit", bare_app_kit_switch_init)
  V("switchState", bare_app_kit_switch_state)

  V("statusItemInit", bare_app_kit_status_item_init)
  V("statusItemAddItem", bare_app_kit_status_item_add_item)
  V("statusItemAddSeparator", bare_app_kit_status_item_add_separator)
  V("statusItemUpdateItem", bare_app_kit_status_item_update_item)
  V("statusItemDestroy", bare_app_kit_status_item_destroy)
#ifdef BARE_APP_KIT_TESTING
  V("statusItemTestingState", bare_app_kit_status_item_testing_state)
  V("statusItemTestingDeleteCount", bare_app_kit_status_item_testing_delete_count)
  V("statusItemTestingPrepare", bare_app_kit_status_item_testing_prepare)
  V("statusItemTestingLateSelect", bare_app_kit_status_item_testing_late_select)
#endif

  V("textInit", bare_app_kit_text_init)
  V("textString", bare_app_kit_text_string)
  V("textEditable", bare_app_kit_text_editable)
  V("textSelectable", bare_app_kit_text_selectable)
  V("textRichText", bare_app_kit_text_rich_text)
  V("textImportsGraphics", bare_app_kit_text_imports_graphics)
  V("textFieldEditor", bare_app_kit_text_field_editor)
  V("textUsesFontPanel", bare_app_kit_text_uses_font_panel)
  V("textDrawsBackground", bare_app_kit_text_draws_background)
  V("textRulerVisible", bare_app_kit_text_ruler_visible)
  V("textAlignment", bare_app_kit_text_alignment)
  V("textBaseWritingDirection", bare_app_kit_text_base_writing_direction)
  V("textSizeToFit", bare_app_kit_text_size_to_fit)
  V("textCopy", bare_app_kit_text_copy)
  V("textCut", bare_app_kit_text_cut)
  V("textDelete", bare_app_kit_text_delete)
  V("textPaste", bare_app_kit_text_paste)
  V("textSelectAll", bare_app_kit_text_select_all)

  V("textFieldInit", bare_app_kit_text_field_init)
  V("textFieldPlaceholderString", bare_app_kit_text_field_placeholder_string)
  V("textFieldBordered", bare_app_kit_text_field_bordered)
  V("textFieldBezeled", bare_app_kit_text_field_bezeled)
  V("textFieldEditable", bare_app_kit_text_field_editable)
  V("textFieldSelectable", bare_app_kit_text_field_selectable)
  V("textFieldDrawsBackground", bare_app_kit_text_field_draws_background)
  V("textFieldBezelStyle", bare_app_kit_text_field_bezel_style)
  V("textFieldMaximumNumberOfLines", bare_app_kit_text_field_maximum_number_of_lines)
  V("textFieldSelectText", bare_app_kit_text_field_select_text)

  V("windowInit", bare_app_kit_window_init)
  V("windowContentView", bare_app_kit_window_content_view)
  V("windowTitlebarAppearsTransparent", bare_app_kit_window_titlebar_appears_transparent)
  V("windowCenter", bare_app_kit_window_center)
  V("windowClose", bare_app_kit_window_close)
  V("windowHidesOnClose", bare_app_kit_window_hides_on_close)
  V("windowHide", bare_app_kit_window_hide)
  V("windowShow", bare_app_kit_window_show)
  V("windowMakeKeyWindow", bare_app_kit_window_make_key_window)
  V("windowOrderBack", bare_app_kit_window_order_back)
  V("windowOrderFront", bare_app_kit_window_order_front)
#undef V

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_int32(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("CONTROL_SIZE_REGULAR", NSControlSizeRegular)
  V("CONTROL_SIZE_SMALL", NSControlSizeSmall)
  V("CONTROL_SIZE_MINI", NSControlSizeMini)
  V("CONTROL_SIZE_LARGE", NSControlSizeLarge)

  V("CONTROL_STATE_MIXED", NSControlStateValueMixed)
  V("CONTROL_STATE_OFF", NSControlStateValueOff)
  V("CONTROL_STATE_ON", NSControlStateValueOn)

  V("BUTTON_TYPE_MOMENTARY_LIGHT", NSButtonTypeMomentaryLight)
  V("BUTTON_TYPE_PUSH_ON_PUSH_OFF", NSButtonTypePushOnPushOff)
  V("BUTTON_TYPE_TOGGLE", NSButtonTypeToggle)
  V("BUTTON_TYPE_SWITCH", NSButtonTypeSwitch)
  V("BUTTON_TYPE_RADIO", NSButtonTypeRadio)
  V("BUTTON_TYPE_MOMENTARY_CHANGE", NSButtonTypeMomentaryChange)
  V("BUTTON_TYPE_ON_OFF", NSButtonTypeOnOff)
  V("BUTTON_TYPE_MOMENTARY_PUSH_IN", NSButtonTypeMomentaryPushIn)
  V("BUTTON_TYPE_ACCELERATOR", NSButtonTypeAccelerator)
  V("BUTTON_TYPE_MULTI_LEVEL_ACCELERATOR", NSButtonTypeMultiLevelAccelerator)

  V("BUTTON_BEZEL_STYLE_PUSH", NSBezelStylePush)
  V("BUTTON_BEZEL_STYLE_FLEXIBLE_PUSH", NSBezelStyleFlexiblePush)
  V("BUTTON_BEZEL_STYLE_DISCLOSURE", NSBezelStyleDisclosure)
  V("BUTTON_BEZEL_STYLE_CIRCULAR", NSBezelStyleCircular)
  V("BUTTON_BEZEL_STYLE_HELP_BUTTON", NSBezelStyleHelpButton)
  V("BUTTON_BEZEL_STYLE_SMALL_SQUARE", NSBezelStyleSmallSquare)
  V("BUTTON_BEZEL_STYLE_TOOLBAR", NSBezelStyleToolbar)
  V("BUTTON_BEZEL_STYLE_ACCESSORY_BAR_ACTION", NSBezelStyleAccessoryBarAction)
  V("BUTTON_BEZEL_STYLE_ACCESSORY_BAR", NSBezelStyleAccessoryBar)
  V("BUTTON_BEZEL_STYLE_PUSH_DISCLOSURE", NSBezelStylePushDisclosure)
  V("BUTTON_BEZEL_STYLE_BADGE", NSBezelStyleBadge)

  V("SLIDER_TYPE_LINEAR", NSSliderTypeLinear)
  V("SLIDER_TYPE_CIRCULAR", NSSliderTypeCircular)

  V("TEXT_ALIGNMENT_LEFT", NSTextAlignmentLeft)
  V("TEXT_ALIGNMENT_RIGHT", NSTextAlignmentRight)
  V("TEXT_ALIGNMENT_CENTER", NSTextAlignmentCenter)
  V("TEXT_ALIGNMENT_JUSTIFIED", NSTextAlignmentJustified)
  V("TEXT_ALIGNMENT_NATURAL", NSTextAlignmentNatural)

  V("TEXT_FIELD_BEZEL_STYLE_SQUARE", NSTextFieldSquareBezel)
  V("TEXT_FIELD_BEZEL_STYLE_ROUNDED", NSTextFieldRoundedBezel)

  V("TICK_MARK_POSITION_BELOW", NSTickMarkPositionBelow)
  V("TICK_MARK_POSITION_ABOVE", NSTickMarkPositionAbove)
  V("TICK_MARK_POSITION_LEADING", NSTickMarkPositionLeading)
  V("TICK_MARK_POSITION_TRAILING", NSTickMarkPositionTrailing)

  V("WINDOW_STYLE_MASK_BORDERLESS", NSWindowStyleMaskBorderless)
  V("WINDOW_STYLE_MASK_TITLED", NSWindowStyleMaskTitled)
  V("WINDOW_STYLE_MASK_CLOSABLE", NSWindowStyleMaskClosable)
  V("WINDOW_STYLE_MASK_MINIATURIZABLE", NSWindowStyleMaskMiniaturizable)
  V("WINDOW_STYLE_MASK_RESIZABLE", NSWindowStyleMaskResizable)
  V("WINDOW_STYLE_MASK_FULL_SCREEN", NSWindowStyleMaskFullScreen)
  V("WINDOW_STYLE_MASK_FULL_SIZE_CONTENT_VIEW", NSWindowStyleMaskFullSizeContentView)

  V("WRITING_DIRECTION_NATURAL", NSWritingDirectionNatural)
  V("WRITING_DIRECTION_LEFT_TO_RIGHT", NSWritingDirectionLeftToRight)
  V("WRITING_DIRECTION_RIGHT_TO_LEFT", NSWritingDirectionRightToLeft)
#undef V

  return exports;
}

BARE_MODULE(bare_app_kit, bare_app_kit_exports)
