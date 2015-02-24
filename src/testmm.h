#pragma once

#include <limits.h>

#include "board.h"
#include "game_config.h"
#include "args.h"



int dummy_heur(const struct board* b);

int test_minimax(const struct board* b,int depth_remaining,int is_max);
int test_negamax(const struct board* b,int depth_remaining,int is_max);
int test_alphabeta(const struct board* b,int depth_remaining,int alpha,int beta);



int arg_run_minimax_tests(struct game_config* gc,const struct parse_state* ps);