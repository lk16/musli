#pragma once

#include <malloc.h>
#include <string.h>

#include "board.h"
#include "human.h"
#include "bot_moves.h"

typedef enum player_type{
  PLAYER_NOT_FOUND, // DO NOT MOVE
  PLAYER_HUMAN,
  PLAYER_BOT_MOVES,
  PLAYER_MAX_ENUM_VALUE, // DO NOT MOVE!
  PLAYER_DEFAULT_BOT = PLAYER_BOT_MOVES
} player_type;

typedef struct player{
  player_type type;
  union{
    human human_data;
    bot_moves moves;
  };
} player;


player_type player_type_get(const char* str);
void player_init(player* p,player_type type,int depth, int perfect_depth);
void player_do_move(player* p,const board* b,board* res);
void player_set_level(player* p,int depth,int perfect_depth);