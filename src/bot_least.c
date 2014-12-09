#include "bot_least.h"

int bot_least_heuristic(const struct board* b)
{
  int me_move_count = board_count_moves(b);
  int opp_move_count = board_count_opponent_moves(b);
  if(me_move_count==0 && opp_move_count==0){
    return 1000 * board_get_disc_diff(b);
  }
  return uint64_count(b->opp) - uint64_count(b->me);
}

void bot_least_init(struct bot_least* bot)
{
  pvs_helper_init(&bot->pvs,bot_least_heuristic,"least");
}

void bot_least_set_level(struct bot_least* bot, int depth, int perfect_depth)
{
  pvs_helper_set_level(&bot->pvs,depth,perfect_depth);
}

void bot_least_do_move(struct bot_least* bot, const struct board* b,struct board* res)
{
  pvs_helper_do_move(&bot->pvs,b,res);
}

