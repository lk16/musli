#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitwise.h"

extern const int board_walk_diff[8][7];
extern const uint64_t board_walk_possible[8][7];

struct board{
  uint64_t me,opp;
};

struct board_with_heur{
  struct board b;
  int heur;
};

int board_with_heur_compare(const void* lhs,const void* rhs);

void board_init(struct board* b);
void board_clear(struct board* b);


uint64_t board_do_move(struct board* b,int move);
void board_undo_move(struct board* b,int move,uint64_t undo);

uint64_t board_get_moves(const struct board* b);
int board_count_moves(const struct board* b);
int board_count_opponent_moves(const struct board* b);

int board_is_valid_move(const struct board* b,int move);
int board_has_valid_moves(const struct board* b);
int board_opponent_has_valid_moves(const struct board* b);

int board_get_disc_diff(const struct board* b);

int board_test_game_ended(const struct board* b);

void board_switch_turn(struct board* b);

struct board* board_get_children(const struct board* b,struct board* out);
struct board_with_heur* board_get_children_with_heur(const struct board* b,struct board_with_heur* out);

int board_only_similar_siblings(const struct board* children,int size);

int board_equals(const struct board* lhs,const struct board* rhs);

void board_do_random_moves(struct board* b,int n);

int board_is_child(const struct board* b,const struct board* child);

void board_print(const struct board* b, FILE* file,int turn);

uint64_t uint64_get_stable(uint64_t bits);

void uint64_print(uint64_t x,FILE* file);