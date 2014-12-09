#pragma once

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <gtk/gtk.h>

#include "board.h"
#include "player.h"
#include "macros.h"
#include "clickable_image.h"

enum game_type{
  GAME_TYPE_MATCH,
  GAME_TYPE_SETUP,
  GAME_TYPE_ANALYSE
};

enum display_type{
  DISPLAY_CONSOLE,
  DISPLAY_WINDOW
};

struct game_state{
  struct board discs;
  int turn;
};


void game_state_init(struct game_state* gs);
void game_state_update_turn(struct game_state* gs);
void game_state_print(const struct game_state* gs,FILE* file);
void game_state_switch_turn(struct game_state* gs);

struct main_window;
void main_window_update_fields(struct main_window*,const struct game_state*);

struct game_config{
  struct player players[2];
  struct game_state history[61];
  int current,redo_max;
  enum game_type type;
  enum display_type display;
  struct main_window* window;
  enum player_type bot_type;
};

void game_config_init(struct game_config* gc,struct main_window* mw);

void game_config_on_new_game(struct game_config* gc);
void game_config_on_ended(const struct game_config* gc);
void game_config_on_any_move(struct game_config* gc,const struct board* child);

struct game_state* game_config_get_state(struct game_config* gc);
const struct game_state* game_config_get_state_const(const struct game_config* gc);
void game_config_show_updated_field(const struct game_config* gc);

struct player* game_config_get_player_to_move(struct game_config* gc);

int game_config_timeout(struct game_config* gc);

void game_config_undo_move(struct game_config* gc);
void game_config_redo_move(struct game_config* gc);

void game_config_console_main(struct game_config* gc);

void game_config_process_click(struct game_config* gc, int index,int button);

void game_config_set_bot(struct game_config* gc,int color,int depth,int perfect_depth);