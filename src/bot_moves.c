#include "bot_moves.h"

int bot_moves_heuristic(const struct board* b)
{
  int res = 0;
  int me_move_count = board_count_moves(b);
  int opp_move_count = board_count_opponent_moves(b);
  if(me_move_count==0 && opp_move_count==0){
    return 1000 * board_get_disc_diff(b);
  }
  res += me_move_count;
  res -= opp_move_count;
  
  res += 3 *(
      uint64_count(b->me & 0x8100000000000081) 
      - uint64_count(b->opp & 0x8100000000000081)
  );
  return res;
}

void bot_moves_init(struct bot_moves* bot)
{
  pvs_helper_init(&bot->pvs,bot_moves_heuristic,"moves");
}

void bot_moves_set_level(struct bot_moves* bot, int depth, int perfect_depth)
{
  pvs_helper_set_level(&bot->pvs,depth,perfect_depth);
}

void bot_moves_do_move(struct bot_moves* bot, const struct board* b,struct board* res)
{
  pvs_helper_do_move(&bot->pvs,b,res);
}