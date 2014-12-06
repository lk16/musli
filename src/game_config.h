#pragma once

#include <assert.h>
#include <string.h>

#include <gtk/gtk.h>

#include "board.h"
#include "player.h"
#include "macros.h"

struct main_window;

typedef enum game_type{
  GAME_TYPE_MATCH,
  GAME_TYPE_SETUP,
  GAME_TYPE_ANALYSE
} game_type;

typedef enum display_type{
  DISPLAY_CONSOLE,
  DISPLAY_WINDOW
} display_type;

typedef struct game_state{
  board discs;
  int turn;
} game_state;


void game_state_init(game_state* gs);
void game_state_update_turn(game_state* gs);
void game_state_print(const game_state* gs, FILE* file);
void game_state_switch_turn(game_state* gs);

typedef struct game_config{
  player players[2];
  game_state history[61];
  int current,redo_max;
  game_type type;
  display_type display;
  struct main_window* window;
} game_config;

void game_config_init(game_config* gc,struct main_window* mw);

void game_config_on_new_game(game_config* gc);
void game_config_on_field_update(game_config* gc);
void game_config_on_ended(const game_config* gc);

game_state* game_config_get_state(game_config* gc);
const game_state* game_config_get_state_const(const game_config* gc);
void game_config_show_updated_field(const game_config* gc);
void game_config_after_move(game_config* gc);

player* game_config_get_player_to_move(game_config* gc);

int game_config_timeout(game_config* gc);

void game_config_undo_move(game_config* gc);
void game_config_redo_move(game_config* gc);

void game_config_console_main(game_config* gc);

int game_config_test_ended(const game_config* gc);

void game_config_process_click(game_config* gc, int index);