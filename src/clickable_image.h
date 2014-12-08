#pragma once

#include <gtk/gtk.h>

#include "game_config.h"

enum{
  LEFT_BUTTON = 1,
  RIGHT_BUTTON = 3
};

struct clickable_image{
  GtkWidget* image;
  GtkWidget* ebox;
  int index;
  struct game_config* gc;
};

void clickable_image_init(
  struct clickable_image* ci,
  const char* file,
  int index,
  struct game_config* gc
);

void clickable_image_on_click(
  GtkWidget *widget,
  GdkEvent* event,
  struct clickable_image* callback_data
);