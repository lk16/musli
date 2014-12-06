#pragma once

#include <gtk/gtk.h>

#include "game_config.h"

struct game_config;

typedef struct clickable_image{
  GtkWidget* image;
  GtkWidget* ebox;
  int index;
  game_config* gc;
} clickable_image;

void clickable_image_init(
  clickable_image* ci,
  const char* file,
  int index,
  struct game_config* gc
);

void clickable_image_on_click(
  GtkWidget *widget,
  GdkEvent* event,
  clickable_image* callback_data
);