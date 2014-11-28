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
void board_undo_move(board* b,uint64_t undo);

uint64_t board_get_moves(const board* b);

int board_is_valid_move(const board* b,int move);
void board_count_moves(const board* b);
int board_has_valid_moves(const board* b);

void board_switch_turn(board* b);