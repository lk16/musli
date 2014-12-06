#pragma once

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>

#include "game_config.h"
#include "clickable_image.h"

typedef struct main_window{
  GtkWidget* window;
  GtkWidget* vbox;
  GtkWidget* table;
  clickable_image images[8][8];
  
  game_config config;
} main_window;


void main_window_init(main_window* mw);
void main_window_table_init(main_window* mw);

void main_window_update_fields(main_window* mw,const game_state* gs);
