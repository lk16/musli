#pragma once

#include <stdio.h>

#include "board.h"
#include "bot_stats.h"
#include "macros.h"

static const int bot_moves_max_sort_depth = 8;

typedef struct bot_moves{
  int depth,perfect_depth;
  board inspected;
  bot_stats stats;
  FILE* out;
  const char* name;
  int moves_left;
} bot_moves;

void bot_moves_init(bot_moves* bot);

void bot_moves_set_level(bot_moves* bot,int depth,int perfect_depth);
void bot_moves_do_move(bot_moves* bot,const board* b,board* res);

int bot_moves_heuristic(const board* b);

int bot_moves_pvs_sorted(bot_moves* bot,int alpha,int beta);
int bot_moves_pvs_unsorted(bot_moves* bot,int alpha,int beta);

int bot_moves_pvs_exact(bot_moves* bot,int alpha,int beta);

int bot_moves_pvs_null_window(bot_moves* bot,int alpha);
int bot_moves_pvs_exact_null_window(bot_moves* bot,int alpha);

void bot_moves_do_move_normally(bot_moves* bot,const board* b, board* res);
void bot_moves_do_move_perfectly(bot_moves* bot,const board* b, board* res);
void bot_moves_do_move_one_possibility(bot_moves* bot,const board* b,board* res);