#include "bot_most.h"

int bot_most_heuristic(const struct board* b)
{
  int me_move_count = board_count_moves(b);
  int opp_move_count = board_count_opponent_moves(b);
  if(me_move_count==0 && opp_move_count==0){
    return 1000 * board_get_disc_diff(b);
  }
  int res = 0;
  res -= uint64_count(b->opp) - uint64_count(b->me);
  res += 3 * uint64_count(b->me & 0x8100000000000081);
  res += -3 * uint64_count(b->opp & 0x8100000000000081);
  return res;
}

void bot_most_init(struct bot_most* bot)
{
  pvs_helper_init(&bot->pvs,bot_most_heuristic,"most");
}

void bot_most_set_level(struct bot_most* bot, int depth, int perfect_depth)
{
  pvs_helper_set_level(&bot->pvs,depth,perfect_depth);
}

void bot_most_do_move(struct bot_most* bot, const struct board* b,struct board* res)
{
  pvs_helper_do_move(&bot->pvs,b,res);
}

