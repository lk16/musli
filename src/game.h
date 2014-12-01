#pragma once

#include <assert.h>

#include "board.h"
#include "player.h"

typedef struct game_state{
  board discs;
  int turn;
} game_state;

typedef struct game{
  player players[2];
  game_state history[60];
  int current,redo_max;
} game;

void game_init(game* g);
void game_reset(game* g);

int game_do_move(game* g,int move);

void game_undo_move(game* g);
void game_redo_move(game* g);

void game_play(game* g);

int game_test_ended(game* g);
void game_over(const game* g);