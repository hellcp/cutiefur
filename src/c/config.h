#pragma once

#include <pebble.h>

enum { DISPLAY_TEXT = 0, DISPLAY_STEPS = 1, DISPLAY_SLEEP = 2 };

enum { DISPLAY_TAP = 0, DISPLAY_ALWAYS = 1, DISPLAY_NEVER = 2 };

enum {
  CREATURE_BEAGLE = 0,
  CREATURE_COW = 1,
  CREATURE_FOX = 2,
  CREATURE_GOAT = 3,
  CREATURE_JERBOA = 4,
  CREATURE_PONY = 5,
  CREATURE_SHARK = 6,
  CREATURE_TABBY = 7,
  CREATURE_TERRIER = 8,
  CREATURE_BUNNY = 9,
  CREATURE_AXOLOTL = 10,
  CREATURE_LOBSTER = 11,
  CREATURE_HEDGEHOG = 12,
  CREATURE_TURTLE = 13
};

typedef struct {
  GColor color_0;
  GColor color_0_contrast;
  GColor color_0_contrast_contrast;
  GColor color_1;
  GColor color_2;
  GColor color_3;
  GColor color_4;
  GColor color_5;
  int creature;
  int display;
  char display_text[10];
  int display_state;
  char date_format[6];
} AppConfig;

void config_load(AppConfig *config);
void config_save(AppConfig *config);
