#pragma once

#include <pebble.h>
#include "config.h"

void ui_init(AppConfig *config);
void ui_deinit();

void ui_update();
void ui_handle_tap();

