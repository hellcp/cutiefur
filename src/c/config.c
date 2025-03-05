#include "config.h"

void config_load(AppConfig *config) {
  // Set default colors
  config->color_0 = GColorBlack;
  config->color_1 = GColorPictonBlue;
  config->color_2 = GColorWhite;
  config->color_3 = GColorMelon;
  config->color_4 = GColorWhite;
  config->color_5 = GColorBlack;
  config->creature = CREATURE_SHARK;
  config->display = DISPLAY_TEXT;
  snprintf(config->display_text, 5, "RAWR");

  // Load colors from storage if available
  if (persist_exists(MESSAGE_KEY_ColorZero)) {
    config->color_0.argb = persist_read_int(MESSAGE_KEY_ColorZero);
  }

  if (persist_exists(MESSAGE_KEY_ColorOne)) {
    config->color_1.argb = persist_read_int(MESSAGE_KEY_ColorOne);
  }

  if (persist_exists(MESSAGE_KEY_ColorTwo)) {
    config->color_2.argb = persist_read_int(MESSAGE_KEY_ColorTwo);
  }

  if (persist_exists(MESSAGE_KEY_ColorThree)) {
    config->color_3.argb = persist_read_int(MESSAGE_KEY_ColorThree);
  }

  if (persist_exists(MESSAGE_KEY_ColorFour)) {
    config->color_4.argb = persist_read_int(MESSAGE_KEY_ColorFour);
  }

  if (persist_exists(MESSAGE_KEY_ColorFive)) {
    config->color_5.argb = persist_read_int(MESSAGE_KEY_ColorFive);
  }

  if (persist_exists(MESSAGE_KEY_Creature)) {
    config->creature = persist_read_int(MESSAGE_KEY_Creature);
  }

  if (persist_exists(MESSAGE_KEY_Display)) {
    config->display = persist_read_int(MESSAGE_KEY_Display);
  }

  if (persist_exists(MESSAGE_KEY_DisplayText)) {
    persist_read_string(MESSAGE_KEY_DisplayText, config->display_text, 9);
  }

  config->color_0_contrast = gcolor_legible_over(config->color_0);
  config->color_0_contrast_contrast = gcolor_legible_over(config->color_0_contrast);
}

void config_save(AppConfig *config) {
  persist_write_int(MESSAGE_KEY_ColorZero, config->color_0.argb);
  persist_write_int(MESSAGE_KEY_ColorOne, config->color_1.argb);
  persist_write_int(MESSAGE_KEY_ColorTwo, config->color_2.argb);
  persist_write_int(MESSAGE_KEY_ColorThree, config->color_3.argb);
  persist_write_int(MESSAGE_KEY_ColorFour, config->color_4.argb);
  persist_write_int(MESSAGE_KEY_ColorFive, config->color_5.argb);
  persist_write_int(MESSAGE_KEY_Creature, config->creature);
  persist_write_int(MESSAGE_KEY_Display, config->display);
  persist_write_string(MESSAGE_KEY_DisplayText, config->display_text);
}

