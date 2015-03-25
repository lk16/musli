#pragma once

#include <limits.h>
#include <malloc.h>

#include "board.h"
#include "game_config.h"
#include "args.h"
#include "bot_stats.h"
#include "hash_table.h"

#define HASHED_VALUE_NO_EXACT_RESULT (9999999)

int dummy_heur(const struct board* b);

void sort_children(struct board* children,struct board* child_end);


int test_pvs(const struct board* b,int depth_remaining,int alpha,int beta);

int test_alphabeta_ht(const struct board* b,struct board_ht* ht, int depth_remaining, int beta);
int test_alphabeta_no_ht(struct board* b, int depth_remaining,int beta);
int test_mtdf_ht(const struct board* b,struct board_ht* ht,int depth_remaining,int f);

int test_alphabeta_exact_ht(const struct board* b,struct board_ht* ht, int depth_remaining, int beta);
int test_alphabeta_exact_no_ht(struct board* b, int depth_remaining,int beta);
int test_mtdf_exact_ht(const struct board* b,struct board_ht* ht,int depth_remaining,int f);



void test_alphabeta_update_hashed_data(const struct board* b,int g,int beta,struct board_ht_data* hashed_data,int depth_remaining);


int test_alphabeta_get_hashed_data(
  const struct board* b,
  struct board_ht* ht,
  int depth_remaining,
  int beta
);


int arg_run_minimax_tests(struct game_config* gc,const struct parse_state* ps);