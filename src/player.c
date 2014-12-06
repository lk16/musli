#include "player.h"

void player_init(player* p, player_type type, int depth, int perfect_depth)
{
  p->type = type;
  switch(type){
    case PLAYER_BOT_MOVES:
      bot_moves_init(&p->moves);
      break;
    case PLAYER_HUMAN:
    default:
      break;
  }
  player_set_level(p,depth,perfect_depth);
}


void player_do_move(player* p, const board* b,board* res)
{
  switch(p->type){
    case PLAYER_HUMAN:
      human_do_move(&p->human_data,b,res);
      break;
    case PLAYER_BOT_MOVES:
      bot_moves_do_move(&p->moves,b,res);
      break;
    default:
      (void)0;
  }
}

void player_set_level(player* p, int depth, int perfect_depth)
{
  switch(p->type){
    case PLAYER_HUMAN:
      break;
    case PLAYER_BOT_MOVES:
      bot_moves_set_level(&p->moves,depth,perfect_depth);
    default:
      break;
  }
}

player_type player_type_get(const char* str)
{
  if(!strcmp(str,"human")) return PLAYER_HUMAN;
  if(!strcmp(str,"moves")) return PLAYER_BOT_MOVES;
  return PLAYER_NOT_FOUND;
}
