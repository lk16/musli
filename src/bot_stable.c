#include "bot_stable.h"

int bot_stable_heuristic(const struct board* b)
{
  int res = 0;
  int me_move_count = board_count_moves(b);
  int opp_move_count = board_count_opponent_moves(b);
  if(me_move_count==0 && opp_move_count==0){
    return 1000 * board_get_disc_diff(b);
  }
  res += me_move_count;
  res -= opp_move_count;
  
  res += 1 * (
      uint64_count(uint64_get_stable(b->me)) 
      - uint64_count(uint64_get_stable(b->opp))
  );
  return res;
}

void bot_stable_init(struct bot_stable* bot)
{
  pvs_helper_init(&bot->pvs,bot_stable_heuristic,"moves");
}

void bot_stable_set_level(struct bot_stable* bot, int depth, int perfect_depth)
{
  pvs_helper_set_level(&bot->pvs,depth,perfect_depth);
}

void bot_stable_do_move(struct bot_stable* bot, const struct board* b,struct board* res)
{
  pvs_helper_do_move(&bot->pvs,b,res);
}