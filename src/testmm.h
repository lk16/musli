#pragma once

#include <limits.h>
#include <malloc.h>

#include "board.h"
#include "game_config.h"
#include "args.h"

#define LAME_HT_SIZE (1048575)

struct lame_ht_data{
  struct board b;
  unsigned low,high;
};

unsigned lame_board_hash(const struct board* b);

struct lame_ht{
  struct lame_ht_data data[LAME_HT_SIZE];
  unsigned(*hash)(const struct board*);
};

struct lame_ht* lame_ht_new(unsigned(*hash)(const struct board*));
void lame_ht_insert(struct lame_ht* ht,const struct board* b,unsigned low,unsigned high);
struct lame_ht_data* lame_ht_find(struct lame_ht* ht,const struct board* b);
void lame_ht_free(struct lame_ht* ht);


int dummy_heur(const struct board* b);

int test_minimax(const struct board* b,int depth_remaining,int is_max);
int test_negamax(const struct board* b,int depth_remaining);
int test_alphabeta(const struct board* b,int depth_remaining,int alpha,int beta);  
int test_pvs(const struct board* b,int depth_remaining,int alpha,int beta);
int test_mtdf(const struct board* b,int depth_remaining,int f);

int test_pvs_ht(const struct board* b,struct lame_ht* ht,int depth_remaining,int alpha,int beta);
int test_mtdf_ht(const struct board* b,struct lame_ht* ht,int depth_remaining,int f);


int arg_run_minimax_tests(struct game_config* gc,const struct parse_state* ps);