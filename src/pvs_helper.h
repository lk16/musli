#pragma once

#include <stdio.h>

#include "board.h"
#include "bot_stats.h"
#include "macros.h"
#include "hash_table.h"

static const int pvs_helper_max_sort_depth = 6;

struct pvs_helper{
  int depth,perfect_depth;
  struct board inspected;
  struct bot_stats stats;
  FILE* out;
  const char* name;
  int moves_left;
  struct hash_table table;
  int(*heuristic)(const struct board* b);
};

void pvs_helper_init(struct pvs_helper* bot,int(*)(const struct board*),const char* name);

void pvs_helper_set_level(struct pvs_helper* bot,int depth,int perfect_depth);
void pvs_helper_do_move(struct pvs_helper* bot,const struct board* b,struct board* res);

int pvs_helper_heuristic(const struct board* b);

int pvs_helper_pvs_sorted(struct pvs_helper* bot,int alpha,int beta);
int pvs_helper_pvs_unsorted(struct pvs_helper* bot,int alpha,int beta);

int pvs_helper_pvs_exact(struct pvs_helper* bot,int alpha,int beta);

int pvs_helper_pvs_null_window(struct pvs_helper* bot,int alpha);
int pvs_helper_pvs_exact_null_window(struct pvs_helper* bot,int alpha);

void pvs_helper_do_move_normally(struct pvs_helper* bot,const struct board* b, struct board* res);
void pvs_helper_do_move_perfectly(struct pvs_helper* bot,const struct board* b, struct board* res);
void pvs_helper_do_move_one_possibility(struct pvs_helper* bot,const struct board* b,struct board* res);