#pragma once

#include <malloc.h>
#include <string.h>

#include "board.h"
#include "human.h"

#include "bot_moves.h"
#include "bot_random.h"
#include "bot_least.h"



enum player_type{
  PLAYER_NOT_FOUND, // DO NOT MOVE
  PLAYER_HUMAN,
  PLAYER_BOT_MOVES,
  PLAYER_BOT_RANDOM,
  PLAYER_BOT_LEAST,
  PLAYER_MAX_ENUM_VALUE, // DO NOT MOVE!
  PLAYER_DEFAULT_BOT = PLAYER_BOT_MOVES
};

struct player{
  enum player_type type;
  union{
    struct human human_data;
    struct bot_moves moves;
    struct bot_random random;
    struct bot_least least;
  };
};

void player_init(struct player* p,enum player_type type,int depth, int perfect_depth);
void player_do_move(struct player* p,const struct board* b,struct board* res);
void player_set_level(struct player* p,int depth,int perfect_depth);