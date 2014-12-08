#include "player.h"

void player_init(struct player* p,enum player_type type, int depth, int perfect_depth)
{
  p->type = type;
  switch(type){
    case PLAYER_BOT_MOVES:
      bot_moves_init(&p->moves);
      break;
    case PLAYER_BOT_RANDOM:
      bot_random_init(&p->random);
      break;
    case PLAYER_BOT_LEAST:
      bot_least_init(&p->least);
      break;
    default:
      break;
  }
  player_set_level(p,depth,perfect_depth);
}


void player_do_move(struct player* p, const struct board* b,struct board* res)
{
  switch(p->type){
    case PLAYER_HUMAN:
      human_do_move(&p->human_data,b,res);
      break;
    case PLAYER_BOT_MOVES:
      bot_moves_do_move(&p->moves,b,res);
      break;
    case PLAYER_BOT_RANDOM:
      bot_random_do_move(&p->random,b,res);
      break;
    case PLAYER_BOT_LEAST:
      bot_least_do_move(&p->least,b,res);
    default:
      (void)0;
  }
}

void player_set_level(struct player* p, int depth, int perfect_depth)
{
  switch(p->type){
    case PLAYER_HUMAN:
      break;
    case PLAYER_BOT_MOVES:
      bot_moves_set_level(&p->moves,depth,perfect_depth);
    case PLAYER_BOT_LEAST:
      bot_least_set_level(&p->least,depth,perfect_depth);
    default:
      break;
  }
}
