#pragma once

#include <stdio.h>

#include "board.h"
#include "bot_stats.h"
#include "pvs_helper.h"

struct bot_stable{
  struct pvs_helper pvs;
};


void bot_stable_init(struct bot_stable* bot);

void bot_stable_set_level(struct bot_stable* bot,int depth,int perfect_depth);

int bot_stable_heuristic(const struct board* b);

void bot_stable_do_move(struct bot_stable* bot,const struct board* b,struct board* res);