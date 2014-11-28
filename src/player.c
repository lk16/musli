#include "player.h"

int player_do_move(player* p, const board* b)
{
  switch(p->type){
    case PLAYER_HUMAN:
      return human_do_move(&p->human_data,b);
    case PLAYER_BOT_MOVES:
    default:
      return -1;
  }
}
