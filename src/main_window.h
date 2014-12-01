#pragma once

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "game_config.h"

typedef struct main_window{
  GtkWidget* window;
  GtkWidget* vbox;
  GtkWidget* table;
  
  game_config config;
} main_window;


void main_window_init(main_window* mw);
