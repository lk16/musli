#pragma once

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>

#include "game_config.h"
#include "clickable_image.h"



struct main_window{
  GtkWidget* window;
  GtkWidget* vbox;
  GtkWidget* table;
  struct clickable_image images[8][8];
  struct game_config config;
};


void main_window_init(struct main_window* mw);
void main_window_table_init(struct main_window* mw);

void main_window_update_fields(struct main_window* mw,const struct game_state* gs);
