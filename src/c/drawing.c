#include "drawing.h"

const int creatures[12][5] = {
    {RESOURCE_ID_IMAGE_BEAGLE_1, RESOURCE_ID_IMAGE_BEAGLE_2,
     RESOURCE_ID_IMAGE_BEAGLE_3, RESOURCE_ID_IMAGE_BEAGLE_4,
     RESOURCE_ID_IMAGE_BEAGLE_5},
    {RESOURCE_ID_IMAGE_COW_1, RESOURCE_ID_IMAGE_COW_2, RESOURCE_ID_IMAGE_COW_3,
     RESOURCE_ID_IMAGE_COW_4, RESOURCE_ID_IMAGE_COW_5},
    {RESOURCE_ID_IMAGE_FOX_1, RESOURCE_ID_IMAGE_FOX_2, RESOURCE_ID_IMAGE_FOX_3,
     RESOURCE_ID_IMAGE_FOX_4, RESOURCE_ID_IMAGE_FOX_5},
    {RESOURCE_ID_IMAGE_GOAT_1, 0, RESOURCE_ID_IMAGE_GOAT_3,
     RESOURCE_ID_IMAGE_GOAT_4, RESOURCE_ID_IMAGE_GOAT_5},
    {RESOURCE_ID_IMAGE_JERBOA_1, RESOURCE_ID_IMAGE_JERBOA_2,
     RESOURCE_ID_IMAGE_JERBOA_3, 0, RESOURCE_ID_IMAGE_JERBOA_5},
    {RESOURCE_ID_IMAGE_PONY_1, RESOURCE_ID_IMAGE_PONY_2, 0,
     RESOURCE_ID_IMAGE_PONY_4, RESOURCE_ID_IMAGE_PONY_5},
    {RESOURCE_ID_IMAGE_SHARK_1, RESOURCE_ID_IMAGE_SHARK_2,
     RESOURCE_ID_IMAGE_SHARK_3, 0, RESOURCE_ID_IMAGE_SHARK_5},
    {RESOURCE_ID_IMAGE_TABBY_1, RESOURCE_ID_IMAGE_TABBY_2, 0, 0,
     RESOURCE_ID_IMAGE_TABBY_5},
    {RESOURCE_ID_IMAGE_TERRIER_1, RESOURCE_ID_IMAGE_TERRIER_2,
     RESOURCE_ID_IMAGE_TERRIER_3, RESOURCE_ID_IMAGE_TERRIER_4,
     RESOURCE_ID_IMAGE_TERRIER_5},
    {RESOURCE_ID_IMAGE_BUNNY_1, RESOURCE_ID_IMAGE_BUNNY_2,
     RESOURCE_ID_IMAGE_BUNNY_3, RESOURCE_ID_IMAGE_BUNNY_4,
     RESOURCE_ID_IMAGE_BUNNY_5},
    {RESOURCE_ID_IMAGE_AXOLOTL_1, 0, RESOURCE_ID_IMAGE_AXOLOTL_3,
     RESOURCE_ID_IMAGE_AXOLOTL_4, RESOURCE_ID_IMAGE_AXOLOTL_5},
    {RESOURCE_ID_IMAGE_LOBSTER_1, RESOURCE_ID_IMAGE_LOBSTER_2, 0, 0,
     RESOURCE_ID_IMAGE_LOBSTER_5}};

typedef struct {
  GColor color;
} ColorCBContext;

static bool prv_draw_command_list_in_color_cb(GDrawCommand *command,
                                              uint32_t index, void *context) {
  ColorCBContext *color = context;
  gdraw_command_set_fill_color(command, color->color);
  return true;
}

void draw_command_image_in_color(GDrawCommandImage *image, GColor color) {
  ColorCBContext ctx = {
      .color = color,
  };
  gdraw_command_list_iterate(gdraw_command_image_get_command_list(image),
                             prv_draw_command_list_in_color_cb, &ctx);
}

void draw_creature_layer(GContext *ctx, int resource, GColor color,
                         GPoint origin) {
  if (resource == 0)
    return;

  GDrawCommandImage *image = gdraw_command_image_create_with_resource(resource);
  draw_command_image_in_color(image, color);
  gdraw_command_image_draw(ctx, image, origin);
  gdraw_command_image_destroy(image);
}
