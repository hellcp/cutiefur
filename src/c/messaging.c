#include "messaging.h"
#include "ui.h"

static AppConfig *s_config;

static void inbox_received_callback(DictionaryIterator *iter, void *context) {
  Tuple *color_0_t = dict_find(iter, MESSAGE_KEY_ColorZero);
  Tuple *color_1_t = dict_find(iter, MESSAGE_KEY_ColorOne);
  Tuple *color_2_t = dict_find(iter, MESSAGE_KEY_ColorTwo);
  Tuple *color_3_t = dict_find(iter, MESSAGE_KEY_ColorThree);
  Tuple *color_4_t = dict_find(iter, MESSAGE_KEY_ColorFour);
  Tuple *color_5_t = dict_find(iter, MESSAGE_KEY_ColorFive);
  Tuple *creature_t = dict_find(iter, MESSAGE_KEY_Creature);
  Tuple *display_t = dict_find(iter, MESSAGE_KEY_Display);
  Tuple *display_state_t = dict_find(iter, MESSAGE_KEY_DisplayState);
  Tuple *display_text_t = dict_find(iter, MESSAGE_KEY_DisplayText);
  Tuple *date_format_t = dict_find(iter, MESSAGE_KEY_DateFormat);

  if (color_0_t) {
    s_config->color_0 = GColorFromHEX(color_0_t->value->int32);
  }
  if (color_1_t) {
    s_config->color_1 = GColorFromHEX(color_1_t->value->int32);
  }
  if (color_2_t) {
    s_config->color_2 = GColorFromHEX(color_2_t->value->int32);
  }
  if (color_3_t) {
    s_config->color_3 = GColorFromHEX(color_3_t->value->int32);
  }
  if (color_4_t) {
    s_config->color_4 = GColorFromHEX(color_4_t->value->int32);
  }
  if (color_5_t) {
    s_config->color_5 = GColorFromHEX(color_5_t->value->int32);
  }
  if (creature_t) {
    s_config->creature = atoi(creature_t->value->cstring);
  }
  if (display_t) {
    s_config->display = atoi(display_t->value->cstring);
  }
  if (display_state_t) {
    s_config->display_state = atoi(display_state_t->value->cstring);
  }
  if (display_text_t) {
    strncpy(s_config->display_text, display_text_t->value->cstring,
            sizeof(s_config->display_text) - 1);
    s_config->display_text[sizeof(s_config->display_text) - 1] = '\0';
  }
  if (date_format_t) {
    strncpy(s_config->date_format, date_format_t->value->cstring,
            sizeof(s_config->date_format) - 1);
    s_config->date_format[sizeof(s_config->date_format) - 1] = '\0';
  }

  s_config->color_0_contrast = gcolor_legible_over(s_config->color_0);
  s_config->color_0_contrast_contrast = gcolor_legible_over(s_config->color_0_contrast);

  config_save(s_config);

  ui_update();
}

void messaging_init(AppConfig *config) {
  s_config = config;

  app_message_register_inbox_received(inbox_received_callback);
  app_message_open(app_message_inbox_size_maximum(),
                   app_message_outbox_size_maximum());
}

void messaging_deinit() {
  app_message_deregister_callbacks();
}

