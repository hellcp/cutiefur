#include <pebble.h>
#include "config.h"
#include "ui.h"
#include "messaging.h"

static AppConfig s_config;

static void init() {
  config_load(&s_config);
  ui_init(&s_config);
  messaging_init(&s_config);
}

static void deinit() {
  config_save(&s_config);
  messaging_deinit();
  ui_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
  return 0;
}

