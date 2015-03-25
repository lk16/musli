#pragma once

#include <stdio.h>

#include "board.h"
#include "bot_stats.h"
#include "mtdf_helper.h"

struct bot_moves{
  struct mtdf_helper mtdf;
};


void bot_moves_init(struct bot_moves* bot);

void bot_moves_set_level(struct bot_moves* bot,int depth,int perfect_depth);

int bot_moves_heuristic(const struct board* b);

void bot_moves_do_move(struct bot_moves* bot,const struct board* b,struct board* res);