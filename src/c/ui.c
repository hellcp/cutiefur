#include "ui.h"
#include "drawing.h"

static Window *s_main_window;
static Layer *s_canvas_layer;
static bool s_tapped;
static GRect s_lasagna;
static AppConfig *s_config;
static AppTimer *s_tap_timer;
#define TAP_TIMEOUT_MS 3000

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GRect obstructed = layer_get_unobstructed_bounds(layer);
  if (bounds.size.h != obstructed.size.h) {
    s_lasagna = GRect(obstructed.size.w - 90, obstructed.size.h - 44, 90, 44);
  } else {
    s_lasagna = GRect(bounds.size.w - PBL_IF_RECT_ELSE(90, 112), bounds.size.h - 64, 90, 64);
  }

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  graphics_context_set_fill_color(ctx, s_config->color_0);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  graphics_context_set_text_color(ctx, s_config->color_0_contrast);
  graphics_context_set_fill_color(ctx, s_config->color_0_contrast);

  // Date
  char date_buffer[6];
  char date_format_buffer[6] = "%m/%d\0";
  strncpy(date_format_buffer, s_config->date_format, sizeof(s_config->date_format));
  strftime(date_buffer, 6, date_format_buffer, t);
  GFont small_leco = fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS);
  GRect date_bounds =
      GRect(0, (obstructed.size.h / 2) - 43, (obstructed.size.w / 2) + 13, 26);
  graphics_draw_text(ctx, date_buffer, small_leco, date_bounds,
                     GTextOverflowModeFill, GTextAlignmentRight, NULL);

  // DoW
  char dow_buffer[4];
  strftime(dow_buffer, 4, "%a", t);
  GFont big_sans = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  GRect dow_bounds =
      GRect((obstructed.size.w / 2) + 17, (obstructed.size.h / 2) - 42,
            obstructed.size.w - ((obstructed.size.w / 2) + 17), 18);
  graphics_draw_text(ctx, dow_buffer, big_sans, dow_bounds,
                     GTextOverflowModeFill, GTextAlignmentLeft, NULL);

  // Battery
  BatteryChargeState battery_state = battery_state_service_peek();
  GRect battery =
      GRect((obstructed.size.w / 2) - 46, (obstructed.size.h / 2) - 19,
            (92 * battery_state.charge_percent) / 100, 2);
  graphics_fill_rect(ctx, battery, 0, GCornerNone);

  // Time
  char time_buffer[9];
  if (clock_is_24h_style()) {
    strftime(time_buffer, 9, "%H:%M", t);
  } else {
    strftime(time_buffer, 9, "%I:%M", t);
  }
  GFont big_leco = fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS);
  GRect time_bounds =
      GRect(0, (obstructed.size.h / 2) - 26, obstructed.size.w, 42);
  graphics_draw_text(ctx, time_buffer, big_leco, time_bounds,
                     GTextOverflowModeFill, GTextAlignmentCenter, NULL);

  // Draw creature
  draw_creature_layer(ctx, creatures[s_config->creature][0], s_config->color_1, s_lasagna.origin);
  draw_creature_layer(ctx, creatures[s_config->creature][1], s_config->color_2, s_lasagna.origin);
  draw_creature_layer(ctx, creatures[s_config->creature][2], s_config->color_3, s_lasagna.origin);
  draw_creature_layer(ctx, creatures[s_config->creature][3], s_config->color_4, s_lasagna.origin);
  draw_creature_layer(ctx, creatures[s_config->creature][4], s_config->color_5, s_lasagna.origin);

  if (s_config->display_state != DISPLAY_NEVER && (s_tapped || s_config->display_state == DISPLAY_ALWAYS)) {
    GDrawCommandImage *bubble_image =
        gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_BUBBLE);
    draw_command_image_in_color(bubble_image, s_config->color_0_contrast);
    GRect bubble_bounds =
        GRect(s_lasagna.origin.x - 52, s_lasagna.origin.y, 52, 64);
    gdraw_command_image_draw(ctx, bubble_image, bubble_bounds.origin);
    gdraw_command_image_destroy(bubble_image);

    char text_buffer[10];
    strncpy(text_buffer, s_config->display_text, sizeof(text_buffer));

    switch (s_config->display) {
    case DISPLAY_STEPS:;
      int32_t steps = health_service_sum_today(HealthMetricStepCount);
      if (steps < 1000) {
        snprintf(text_buffer, 5, "%ld", steps);
      } else if (steps < 10000) {
        int steps_major = steps / 1000;
        int steps_minor = (steps % 1000) / 100;
        if (steps_minor == 0) {
          snprintf(text_buffer, 3, "%dK", steps_major);
        } else {
          snprintf(text_buffer, 5, "%d.%dK", steps_major, steps_minor);
        }
      } else {
        snprintf(text_buffer, 9, "%ldK", steps / 1000);
      }
      break;
    case DISPLAY_SLEEP:;
      int32_t sleep_seconds =
          health_service_sum_today(HealthMetricSleepSeconds);
      int sleep_hours = sleep_seconds / 3600;
      int sleep_minutes = (sleep_seconds % 3600) / 60;
      snprintf(text_buffer, 6, "%d:%d", sleep_hours, sleep_minutes);
      break;
    default:
      break;
    }

    graphics_context_set_text_color(ctx, s_config->color_0_contrast_contrast);
    GFont small_sans = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
    GRect bubble_text_bounds =
        GRect(bubble_bounds.origin.x + 6, bubble_bounds.origin.y + 12, 43, 18);
    graphics_draw_text(ctx, text_buffer, small_sans, bubble_text_bounds,
                       GTextOverflowModeFill, GTextAlignmentCenter, NULL);
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(s_canvas_layer);
}

static void tap_timeout_callback(void *context) {
  s_tap_timer = NULL;
  s_tapped = false;
  layer_mark_dirty(s_canvas_layer);
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  if (s_tap_timer) {
    app_timer_cancel(s_tap_timer);
    s_tap_timer = NULL;
  }

  s_tapped = true;

  s_tap_timer = app_timer_register(TAP_TIMEOUT_MS, tap_timeout_callback, NULL);
  
  layer_mark_dirty(s_canvas_layer);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_tapped = false;

  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_layer, s_canvas_layer);
}

static void main_window_unload(Window *window) {
  layer_destroy(s_canvas_layer);
}

void ui_init(AppConfig *config) {
  s_config = config;
  s_tap_timer = NULL;

  s_main_window = window_create();
  window_set_window_handlers(
      s_main_window,
      (WindowHandlers){.load = main_window_load, .unload = main_window_unload});
  window_stack_push(s_main_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  accel_tap_service_subscribe(tap_handler);
}

void ui_deinit() {
  if (s_tap_timer) {
    app_timer_cancel(s_tap_timer);
    s_tap_timer = NULL;
  }

  window_destroy(s_main_window);

  tick_timer_service_unsubscribe();
  accel_tap_service_unsubscribe();
}

void ui_update() {
  if (s_canvas_layer) {
    layer_mark_dirty(s_canvas_layer);
  }
}

