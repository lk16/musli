#pragma once

#include <math.h>

#include "board.h"
#include "player.h"
#include "player_name.h"

#define MAX_TOURNAMENT_ENTRANTS (100)
#define TOURNAMENT_INIT_RATING (1500)
#define K_FACTOR (32.0)

enum game_result{
  WHITE_WINS,
  BLACK_WINS,
  DRAW
};

struct entrant{
  struct player p;
  int rating;
  char name[40];
};

struct tournament{
  int num_entrants;
  struct entrant entrants[MAX_TOURNAMENT_ENTRANTS];
};

enum game_result play_othello_game(struct board b,struct player* black,struct player* white);

void entrant_update_rating(struct entrant* black,struct entrant* white,enum game_result res);

void play_tournament(struct tournament* t);

void show_tournament_stats(const struct tournament* t);

int entrant_compare(const void* lhs,const void* rhs);

void tournament_add(struct tournament* t,enum player_type type,int depth,int perfect_depth,FILE* out);