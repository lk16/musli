#pragma once

#include <malloc.h>

#include "board.h"
#include "human.h"
#include "bot_moves.h"

typedef enum player_type{
  PLAYER_HUMAN,
  PLAYER_BOT_MOVES
} player_type;

typedef struct player{
  player_type type;
  union{
    human human_data;
    bot_moves moves;
  };
} player;


void player_init(player* p);
void player_do_move(player* p,const board* b,board* res);
void player_set_level(player* p,int depth,int perfect_depth);