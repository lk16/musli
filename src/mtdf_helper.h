#pragma once

#include <stdio.h>

#include "board.h"
#include "bot_stats.h"
#include "macros.h"
#include "hash_table.h"

#define MTDF_MIN_HASHTABLE_DEPTH 7

struct mtdf_helper{
  int depth,perfect_depth;
  struct board inspected;
  struct bot_stats stats;
  struct board_ht* hashtable;
  FILE* out;
  const char* name;
  int moves_left;
  int(*heuristic)(const struct board* b);
};

void mtdf_helper_init(struct mtdf_helper* m,int(*)(const struct board*),const char* name);

void mtdf_helper_set_level(struct mtdf_helper* m,int depth,int perfect_depth);
void mtdf_helper_do_move(struct mtdf_helper* m,const struct board* b,struct board* res);

int mtdf_helper_heuristic(const struct board* b);

int mtdf_helper_alphabeta_main(struct mtdf_helper* m,int f);
int mtdf_helper_alphabeta_ht(struct mtdf_helper* m,int beta);
int mtdf_helper_alphabeta_no_ht(struct mtdf_helper* m,int beta);

int mtdf_helper_alphabeta_exact_main(struct mtdf_helper* m,int f);
int mtdf_helper_alphabeta_exact_ht(struct mtdf_helper* m,int beta);
int mtdf_helper_alphabeta_exact_no_ht(struct mtdf_helper* m,int beta);

void mtdf_helper_do_move_normally(struct mtdf_helper* m,const struct board* b, struct board* res);
void mtdf_helper_do_move_perfectly(struct mtdf_helper* m,const struct board* b, struct board* res);
void mtdf_helper_do_move_one_possibility(struct mtdf_helper* m,const struct board* b,struct board* res);