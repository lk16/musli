#pragma once

#include <stdio.h>

#include "board.h"
#include "bot_stats.h"
#include "pvs_helper.h"

struct bot_least{
  struct pvs_helper pvs;
};


void bot_least_init(struct bot_least* bot);

void bot_least_set_level(struct bot_least* bot,int depth,int perfect_depth);

int bot_least_heuristic(const struct board* b);

void bot_least_do_move(struct bot_least* bot,const struct board* b,struct board* res);