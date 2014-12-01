#pragma once

#include <stdint.h>
#include <stdio.h>

#include "bitwise.h"

extern const int board_walk_diff[8][7];
extern const uint64_t board_walk_possible[8][7];

typedef struct board{
  uint64_t me,opp;
} board;

void board_init(board* b);
void board_clear(board* b);

void board_print(const board* b,FILE* file,int turn);


uint64_t board_do_move(board* b,int move);
void board_undo_move(board* b,int move,uint64_t undo);

uint64_t board_get_moves(const board* b);
int board_count_moves(const board* b);
int board_count_opponent_moves(const board* b);

int board_is_valid_move(const board* b,int move);
int board_has_valid_moves(const board* b);

int board_get_disc_diff(const board* b);

void board_switch_turn(board* b);

board* board_get_children(const board* b,board* out);

int board_only_similar_siblings(const board* children,int size);