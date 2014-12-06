#include "board.h"

const int board_walk_diff[8][7] = {
  {- 9,-18,-27,-36,-45,-54,-63}, // 0: up left
  {- 8,-16,-24,-32,-40,-48,-56}, // 1: up 
  {- 7,-14,-21,-28,-35,-42,-49}, // 2: up right
  {- 1,- 2,- 3,- 4,- 5,- 6,- 7}, // 3: left
  {  1,  2,  3,  4,  5,  6,  7}, // 4: right
  {  7, 14, 21, 28, 35, 42, 49}, // 5: down left
  {  8, 16, 24, 32, 40, 48, 56}, // 6: down
  {  9, 18, 27, 36, 45, 54, 63}  // 7: down right
};

const uint64_t board_walk_possible[8][7] = {
  { // up left
    0xFEFEFEFEFEFEFE00,
    0xFCFCFCFCFCFC0000,
    0xF8F8F8F8F8000000,
    0xF0F0F0F000000000,
    0xE0E0E00000000000,
    0xC0C0000000000000,
    0x8000000000000000
  },
  { // up
    0xFFFFFFFFFFFFFF00,
    0xFFFFFFFFFFFF0000,
    0xFFFFFFFFFF000000,
    0xFFFFFFFF00000000,
    0xFFFFFF0000000000,
    0xFFFF000000000000,
    0xFF00000000000000
  },
  { // up right
    0x7F7F7F7F7F7F7F00,
    0x3F3F3F3F3F3F0000,
    0x1F1F1F1F1F000000,
    0x0F0F0F0F00000000,
    0x0707070000000000,
    0x0303000000000000,
    0x0100000000000000
  },
  { // left
    0xFEFEFEFEFEFEFEFE,
    0xFCFCFCFCFCFCFCFC,
    0xF8F8F8F8F8F8F8F8,
    0xF0F0F0F0F0F0F0F0,
    0xE0E0E0E0E0E0E0E0,
    0xC0C0C0C0C0C0C0C0,
    0x8080808080808080
  },
  { // right
    0x7F7F7F7F7F7F7F7F,
    0x3F3F3F3F3F3F3F3F,
    0x1F1F1F1F1F1F1F1F,
    0x0F0F0F0F0F0F0F0F,
    0x0707070707070707,
    0x0303030303030303,
    0x0101010101010101
  },
  { // down left
    0x00FEFEFEFEFEFEFE,
    0x0000FCFCFCFCFCFC,
    0x000000F8F8F8F8F8,
    0x00000000F0F0F0F0,
    0x0000000000E0E0E0,
    0x000000000000C0C0,
    0x0000000000000080
  },
  { // down
    0x00FFFFFFFFFFFFFF,
    0x0000FFFFFFFFFFFF,
    0x000000FFFFFFFFFF,
    0x00000000FFFFFFFF,
    0x0000000000FFFFFF,
    0x000000000000FFFF,
    0x00000000000000FF
  },
  { // down right
    0x007F7F7F7F7F7F7F,
    0x00003F3F3F3F3F3F,
    0x0000001F1F1F1F1F,
    0x000000000F0F0F0F,
    0x0000000000070707,
    0x0000000000000303,
    0x0000000000000001
  }
};


void board_clear(board* b)
{
  b->me = b->opp = 0ull;
}

void board_init(board* b)
{
  b->opp = (1ull << 27) | (1ull << 36);
  b->me = (1ull << 28) | (1ull << 35);
}



uint64_t board_get_moves(const board* b)
{
  uint64_t res = 0ull;
  
  for(int d=4;d<8;d++){
    res |= 
    (
    ((b->opp >> board_walk_diff[d][0]) & board_walk_possible[d][0]) 
    & 
    (
    ((b->me >> board_walk_diff[d][1]) & board_walk_possible[d][1])
    |
    (
    ((b->opp >> board_walk_diff[d][1]) & board_walk_possible[d][1])
    &
    (
    ((b->me >> board_walk_diff[d][2]) & board_walk_possible[d][2])
    |
    (
    ((b->opp >> board_walk_diff[d][2]) & board_walk_possible[d][2])
    &
    (
    ((b->me >> board_walk_diff[d][3]) & board_walk_possible[d][3])
    |
    (
    ((b->opp >> board_walk_diff[d][3]) & board_walk_possible[d][3])
    &
    (
    ((b->me >> board_walk_diff[d][4]) & board_walk_possible[d][4])
    |
    (
    ((b->opp >> board_walk_diff[d][4]) & board_walk_possible[d][4])
    &
    (
    ((b->me >> board_walk_diff[d][5]) & board_walk_possible[d][5])
    |
    (
    ((b->opp >> board_walk_diff[d][5]) & board_walk_possible[d][5])
    &
    ((b->me >> board_walk_diff[d][6]) & board_walk_possible[d][6])
    )
    )
    )
    )  
    )
    )
    )
    )
    )
    )
    );
    
    res |= 
    (
    ((b->opp << board_walk_diff[d][0]) & board_walk_possible[7-d][0]) 
    & 
    (
    ((b->me << board_walk_diff[d][1]) & board_walk_possible[7-d][1])
    |
    (
    ((b->opp << board_walk_diff[d][1]) & board_walk_possible[7-d][1])
    &
    (
    ((b->me << board_walk_diff[d][2]) & board_walk_possible[7-d][2])
    |
    (
    ((b->opp << board_walk_diff[d][2]) & board_walk_possible[7-d][2])
    &
    (
    ((b->me << board_walk_diff[d][3]) & board_walk_possible[7-d][3])
    |
    (
    ((b->opp << board_walk_diff[d][3]) & board_walk_possible[7-d][3])
    &
    (
    ((b->me << board_walk_diff[d][4]) & board_walk_possible[7-d][4])
    |
    (
    ((b->opp << board_walk_diff[d][4]) & board_walk_possible[7-d][4])
    &
    (
    ((b->me << board_walk_diff[d][5]) & board_walk_possible[7-d][5])
    |
    (
    ((b->opp << board_walk_diff[d][5]) & board_walk_possible[7-d][5])
    &
    ((b->me << board_walk_diff[d][6]) & board_walk_possible[7-d][6])
    )
    )
    )
    )  
    )
    )
    )
    )
    )
    )
    );
  }
  
  res &= ~(b->me | b->opp);
  return res;
}

int board_count_opponent_moves(const board* b)
{
  uint64_t res = 0ull;
  
  for(int d=4;d<8;d++){
    res |= 
    (
    ((b->me >> board_walk_diff[d][0]) & board_walk_possible[d][0]) 
    & 
    (
    ((b->opp >> board_walk_diff[d][1]) & board_walk_possible[d][1])
    |
    (
    ((b->me >> board_walk_diff[d][1]) & board_walk_possible[d][1])
    &
    (
    ((b->opp >> board_walk_diff[d][2]) & board_walk_possible[d][2])
    |
    (
    ((b->me >> board_walk_diff[d][2]) & board_walk_possible[d][2])
    &
    (
    ((b->opp >> board_walk_diff[d][3]) & board_walk_possible[d][3])
    |
    (
    ((b->me >> board_walk_diff[d][3]) & board_walk_possible[d][3])
    &
    (
    ((b->opp >> board_walk_diff[d][4]) & board_walk_possible[d][4])
    |
    (
    ((b->me >> board_walk_diff[d][4]) & board_walk_possible[d][4])
    &
    (
    ((b->opp >> board_walk_diff[d][5]) & board_walk_possible[d][5])
    |
    (
    ((b->me >> board_walk_diff[d][5]) & board_walk_possible[d][5])
    &
    ((b->opp >> board_walk_diff[d][6]) & board_walk_possible[d][6])
    )
    )
    )
    )  
    )
    )
    )
    )
    )
    )
    );
    
    res |= 
    (
    ((b->me << board_walk_diff[d][0]) & board_walk_possible[7-d][0]) 
    & 
    (
    ((b->opp << board_walk_diff[d][1]) & board_walk_possible[7-d][1])
    |
    (
    ((b->me << board_walk_diff[d][1]) & board_walk_possible[7-d][1])
    &
    (
    ((b->opp << board_walk_diff[d][2]) & board_walk_possible[7-d][2])
    |
    (
    ((b->me << board_walk_diff[d][2]) & board_walk_possible[7-d][2])
    &
    (
    ((b->opp << board_walk_diff[d][3]) & board_walk_possible[7-d][3])
    |
    (
    ((b->me << board_walk_diff[d][3]) & board_walk_possible[7-d][3])
    &
    (
    ((b->opp << board_walk_diff[d][4]) & board_walk_possible[7-d][4])
    |
    (
    ((b->me << board_walk_diff[d][4]) & board_walk_possible[7-d][4])
    &
    (
    ((b->opp << board_walk_diff[d][5]) & board_walk_possible[7-d][5])
    |
    (
    ((b->me << board_walk_diff[d][5]) & board_walk_possible[7-d][5])
    &
    ((b->opp << board_walk_diff[d][6]) & board_walk_possible[7-d][6])
    )
    )
    )
    )  
    )
    )
    )
    )
    )
    )
    );
  }
  
  res &= ~(b->opp | b->me);
  return uint64_count(res);
}

int board_is_valid_move(const board* b, int move)
{
  uint64_t moves = board_get_moves(b);
  return (moves & (1ull << move))!=0;
}

uint64_t board_do_move(board* b,int move){
  uint64_t line,flipped = 0ull;
  int end;
  
  uint64_t left_border_mask,right_border_mask;
  left_border_mask = right_border_mask = 0x0;
  
  switch(move%8){
    case 0: right_border_mask = 0xFEFEFEFEFEFEFEFE; break;
    case 1: right_border_mask = 0xFCFCFCFCFCFCFCFC; break;
    case 2: right_border_mask = 0xF8F8F8F8F8F8F8F8; break;
    case 3: right_border_mask = 0xF0F0F0F0F0F0F0F0; break;
    case 4: right_border_mask = 0xE0E0E0E0E0E0E0E0; break;
    case 5: right_border_mask = 0xC0C0C0C0C0C0C0C0; break;
    default: right_border_mask = 0x0; break;
  }
  
  switch(move%8){
    case 2: left_border_mask = 0x0303030303030303; break;
    case 3: left_border_mask = 0x0707070707070707; break;
    case 4: left_border_mask = 0x0F0F0F0F0F0F0F0F; break;
    case 5: left_border_mask = 0x1F1F1F1F1F1F1F1F; break;
    case 6: left_border_mask = 0x3F3F3F3F3F3F3F3F; break;
    case 7: left_border_mask = 0x7F7F7F7F7F7F7F7F; break;
    default: left_border_mask = 0x0; break;
  }
  
  /* down */
  if(move/8 < 6){
    line = 0x0101010101010100l << move;
    end = uint64_find_first(line & b->me);
    line &= uint64_before[end];
    if((b->opp & line) == line){
      flipped |= line;
    }
  }
  
  /* up */
  if(move/8 > 1){
    line = 0x0080808080808080l >> (63-move);
    end = uint64_find_last(line & b->me);
    line &= uint64_after[end];
    if((b->opp & line) == line){
      flipped |= line;
    }
  }
  
  /* left */
  if(move%8 > 1){
    line = (0x7F00000000000000l >> (63-move)) & left_border_mask;
    end = uint64_find_last(line & b->me);
    line &= uint64_after[end];
    if((b->opp & line) == line){
      flipped |= line;
    }
  }
  
  /* right */
  if(move%8 < 6){
    line = (0x00000000000000FEl << move) & right_border_mask;
    end = uint64_find_first(line & b->me);
    line &= uint64_before[end];
    if((b->opp & line) == line){
      flipped |= line;
    }
  }
  
  /* right down */
  if((move%8 < 6) && (move/8 < 6)){
    line = (0x8040201008040200 << move) & right_border_mask;
    end = uint64_find_first(line & b->me);
    line &= uint64_before[end];
    if((b->opp & line) == line){
      flipped |= line;
    }
  }
  
  /* left up */
  if((move%8 > 1) && (move/8 > 1)){
    line = (0x0040201008040201 >> (63-move)) & left_border_mask;
    end = uint64_find_last(line & b->me);
    line &= uint64_after[end];
    if((b->opp & line) == line){
      flipped |= line;
    }
  }
  
  /* right up */
  if((move%8 < 6) && (move/8 > 1)){
    if(move<=56){
      line = (0x0002040810204080 >> (56-move)) & right_border_mask;
    }
    else{
      line = (0x0002040810204080 << (move-56)) & right_border_mask;
    }
    end = uint64_find_last(line & b->me);
    line &= uint64_after[end];
     if((b->opp & line) == line){
      flipped |= line;
    }
  }
  
  /* left down */
  if((move%8 > 1) && (move/8 < 6)){
    if(move>=7){
      line = (0x0102040810204000 << (move-7)) & left_border_mask;
    }
    else{
      line = (0x0102040810204000 >> (7-move)) & left_border_mask;
    }
    end = uint64_find_first(line & b->me);
    line &= uint64_before[end];
    if((b->opp & line) == line){
      flipped |= line;
    }
  }
  
  b->me |= uint64_set[move] | flipped;
  b->opp &= ~b->me;
  board_switch_turn(b);  
  return flipped; 
}

void board_switch_turn(board* b)
{
  uint64_t tmp = b->opp;
  b->opp = b->me;
  b->me = tmp;
}

int board_count_moves(const board* b)
{
  return uint64_count(board_get_moves(b));
}

int board_has_valid_moves(const board* b)
{
  return board_get_moves(b)!=0ull;
}

int board_opponent_has_valid_moves(const board* b)
{
  return board_count_opponent_moves(b)!=0;
}


int board_get_disc_diff(const board* b) 
{
  int count[2];
  
  count[0] = uint64_count(b->me);
  count[1] = uint64_count(b->opp);
  
  if(count[0] > count[1]){ /* I win */
    return 64 - (2*count[1]);
  }
  else if(count[0] < count[1]){ /* Opp wins */
    return -64 + (2*count[0]);
  }
  else{ /* draw */
    return 0;
  }
}

board* board_get_children(const board* b, board* out)
{
  board* out_end = out;
  uint64_t valid_moves =  board_get_moves(b);
  
  while(valid_moves != 0ull){
    int move_id = uint64_find_first(valid_moves);
    *out_end = *b;
    board_do_move(out_end,move_id);
    out_end++;
    valid_moves &= uint64_reset[move_id]; 
  }
  return out_end;
}

int board_only_similar_siblings(const board* children, int size)
{
  (void)children;
  (void)size;
  return 0;
}

void board_undo_move(board* b,int move,uint64_t undo)
{
  uint64_t tmp = b->me;
  b->me = b->opp & ~(undo | uint64_set[move]);
  b->opp = tmp | undo;
}

int board_test_game_ended(const board* b)
{
  return !(board_has_valid_moves(b) || board_opponent_has_valid_moves(b));
}
