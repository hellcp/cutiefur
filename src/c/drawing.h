#pragma once

#include <pebble.h>

extern const int creatures[11][5];

void draw_command_image_in_color(GDrawCommandImage *image, GColor color);
void draw_creature_layer(GContext *ctx, int resource, GColor color,
                         GPoint origin);
