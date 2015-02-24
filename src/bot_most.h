#pragma once

#include <stdio.h>

#include "board.h"
#include "bot_stats.h"
#include "pvs_helper.h"

struct bot_most{
  struct pvs_helper pvs;
};


void bot_most_init(struct bot_most* bot);

void bot_most_set_level(struct bot_most* bot,int depth,int perfect_depth);

int bot_most_heuristic(const struct board* b);

void bot_most_do_move(struct bot_most* bot,const struct board* b,struct board* res);
