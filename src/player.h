#pragma once

#include "board.h"
#include "human.h"

typedef enum player_type{
  PLAYER_HUMAN,
  PLAYER_BOT_MOVES
} player_type;

typedef struct player{
  player_type type;
  union{
    human human_data;
  };
} player;

int player_do_move(player* p,const board* b);