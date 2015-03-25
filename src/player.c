#include "player.h"

void player_init(struct player* p,enum player_type type, int depth, int perfect_depth)
{
  if(depth<=0 || depth>60 || perfect_depth<=0 || perfect_depth>60){
    printf("Player init error: (d,pd) = (%d,%d)\n",depth,perfect_depth);
    exit(1);
  }
  
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
    case PLAYER_BOT_STABLE:
      bot_stable_init(&p->stable);
      break;
    case PLAYER_BOT_MOST:
      bot_most_init(&p->most);
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
      break;
    case PLAYER_BOT_STABLE:
      bot_stable_do_move(&p->stable,b,res);
      break;
    case PLAYER_BOT_MOST:
      bot_most_do_move(&p->most,b,res);
      break;
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
      break;
    case PLAYER_BOT_LEAST:
      bot_least_set_level(&p->least,depth,perfect_depth);
      break;
    case PLAYER_BOT_STABLE:
      bot_stable_set_level(&p->stable,depth,perfect_depth);
      break;
    case PLAYER_BOT_MOST:
      bot_most_set_level(&p->most,depth,perfect_depth);
    default:
      break;
  }
}

void player_redirect_output(struct player* p, FILE* out)
{
  switch(p->type){
    case PLAYER_HUMAN:
      break;
    case PLAYER_BOT_MOVES:
      p->moves.mtdf.out = out;
      break;
    case PLAYER_BOT_LEAST:
      p->least.pvs.out = out;
      break;
    case PLAYER_BOT_STABLE:
      p->stable.pvs.out = out;
      break;
    case PLAYER_BOT_MOST:
      p->most.pvs.out = out;
      break;
    default:
      break;
  }
}
