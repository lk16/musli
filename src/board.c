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


void board_clear(struct board* b)
{
  b->me = b->opp = 0ull;
}

void board_init(struct board* b)
{
  b->opp = (1ull << 27) | (1ull << 36);
  b->me = (1ull << 28) | (1ull << 35);
}



uint64_t board_get_moves(const struct board* b)
{
  uint64_t res = 0ull;
  
  int d;
  for(d=4;d<8;d++){
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

int board_count_opponent_moves(const struct board* b)
{
  uint64_t res = 0ull;
  
  int d;
  for(d=4;d<8;d++){
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

int board_is_valid_move(const struct board* b, int move)
{
  uint64_t moves = board_get_moves(b);
  return (moves & (1ull << move))!=0;
}

uint64_t board_do_move(struct board* b,int move){
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

void board_switch_turn(struct board* b)
{
  generic_swap(b->me,b->opp);
}

int board_count_moves(const struct board* b)
{
  return uint64_count(board_get_moves(b));
}

int board_has_valid_moves(const struct board* b)
{
  return board_get_moves(b)!=0ull;
}

int board_opponent_has_valid_moves(const struct board* b)
{
  return board_count_opponent_moves(b)!=0;
}


int board_get_disc_diff(const struct board* b) 
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

struct board* board_get_children(const struct board* b, struct board* out)
{ 
  struct board* out_end = out;
  uint64_t valid_moves =  board_get_moves(b);
  int i;
  for(i=0;i<32;i++){
    out[i] = *b;
  }
  while(valid_moves != 0ull){
    int move_id = uint64_find_first(valid_moves);
    //*out_end = *b;
    board_do_move(out_end,move_id);
    out_end++;
    valid_moves &= uint64_reset[move_id]; 
  }
  return out_end;
}

int board_only_similar_siblings(const struct board* children, int size)
{
  (void)children;
  (void)size;
  return 0;
}

void board_undo_move(struct board* b,int move,uint64_t undo)
{
  uint64_t tmp = b->me;
  b->me = b->opp & ~(undo | uint64_set[move]);
  b->opp = tmp | undo;
}

int board_test_game_ended(const struct board* b)
{
  return !(board_has_valid_moves(b) || board_opponent_has_valid_moves(b));
}


int board_equals(const struct board* lhs, const struct board* rhs)
{
  return lhs->me==rhs->me && lhs->opp==rhs->opp;
}

void board_do_random_moves(struct board* b, int n)
{
  struct board children[32];
  struct board* child_end;
  int i;
  for(i=0;i<n;i++){
    if(board_test_game_ended(b)){
      break;
    }
    child_end = board_get_children(b,children);
    if(child_end == children){
      board_switch_turn(b);
      child_end = board_get_children(b,children);
    }
    *b = children[rand() % (child_end-children)];
  }
}

int board_is_child(const struct board* b,const struct board* child)
{
  struct board children[32];
  struct board* child_end;
  int i;
  child_end = board_get_children(b,children);
  for(i=0;i<(child_end-children);i++){
    if(board_equals(child,children+i)){
      return 1;
    }
  }
  return 0;
}

struct board_with_heur* board_get_children_with_heur(const struct board* b,
                                                     struct board_with_heur* out)
{
  struct board_with_heur* out_end = out;
  uint64_t valid_moves =  board_get_moves(b);
  while(valid_moves != 0ull){
    int move_id = uint64_find_first(valid_moves);
    out_end->b = *b;
    out_end->heur = 0;
    board_do_move(&out_end->b,move_id);
    out_end++;
    valid_moves &= uint64_reset[move_id]; 
  }
  return out_end;
}

int board_with_heur_compare(const void* lhs, const void* rhs)
{
  return ((struct board_with_heur*)lhs)->heur - ((struct board_with_heur*)rhs)->heur;
}

void board_print(const struct board* b, FILE* file,int turn)
{
  fprintf(file,"%s","+-a-b-c-d-e-f-g-h-+\n");
  int f;
  uint64_t moves,white,black;
  moves = board_get_moves(b);
  black = turn ? b->me : b->opp;
  white = turn ? b->opp : b->me;
  for(f=0;f<64;f++){
    if(f%8 == 0){
      fprintf(file,"%d ",(f/8)+1);
    }
    if(white & (1ull << f)){
      fprintf(file,"%s","\033[31;1m\u2B24\033[0m ");
    }
    else if(black & (1ull << f)){
      fprintf(file,"%s","\033[34;1m\u2B24\033[0m ");
    }
    else if(moves & (1ull << f)){
      fprintf(file,"%s", "- ");
    }  
    else{
      fprintf(file,"%s","  ");
    }
    if(f%8 == 7){
      fprintf(file,"%s","|\n");
    }
  }
  fprintf(file,"%s","+-----------------+\n");
}

void uint64_print(uint64_t x, FILE* file)
{
  fprintf(file,"%s","+-a-b-c-d-e-f-g-h-+\n");
  int f;
  for(f=0;f<64;f++){
    if(f%8 == 0){
      fprintf(file,"%d ",(f/8)+1);
    }
    fprintf(file,"%c ",(x & uint64_set[f]) ? '@' : ' ' );
    if(f%8 == 7){
      fprintf(file,"%s","|\n");
    }
  }
  fprintf(file,"%s","+-----------------+\n");
}


uint64_t uint64_get_stable(uint64_t input)
{
  uint64_t stable = 0ull;
  int i,j;

  static const int diagonal_tables[4][8][8] = 
  {
    {
      {0},
      {1, 8},
      {2, 9,16},
      {3,10,17,24},
      {4,11,18,25,32},
      {5,12,19,26,33,40},
      {6,13,20,27,34,41,48},
      {7,14,21,28,35,42,49,56}
    },
    { 
      {7},
      {6,15},
      {5,14,23},
      {4,13,22,31},
      {3,12,21,30,39},
      {2,11,20,29,38,47},
      {1,10,19,28,37,46,55},
      {0, 9,18,27,36,45,54,63}
    },
    { 
      {56},
      {48,57},
      {40,49,58},
      {32,41,50,59},
      {24,33,42,51,60},
      {16,25,34,43,52,61},
      { 8,17,26,35,44,53,62},
      { 0, 9,18,27,36,45,54,63}
    },
    { 
      {63},
      {55,62},
      {47,54,61},
      {39,46,53,60},
      {31,38,45,52,59},
      {23,30,37,44,51,58},
      {15,22,29,36,43,50,57},
      { 7,14,21,28,35,42,49,56}
    }
  };
  
  uint64_t bit;
  for(i=0;i<8;i++){
    for(j=0;j<i+1;j++){
      bit = uint64_set[diagonal_tables[0][i][j]];
      if(bit & input){
        if(bit & 0x00000000000001){
          stable |= 0x00000000000001;
        }
        else if(bit & 0x01010101010100){
          stable |= (bit & (stable << 8));
        }
        else if(bit & (stable << 1)){
          stable |= bit;
        }
        else{
          break;
        }
      }
      else{
        break;
      }
    }
    for(j=i;j>=0;j--){
      bit = uint64_set[diagonal_tables[0][i][j]];
      if(bit & input){
        if(bit & 0x0000000000000001){
          stable |= 0x0000000000000001;
        }
        else if(bit & 0x00000000000000FE){
          stable |= (bit & (stable << 1));
        }
        else if(bit & (stable << 8)){
          stable |= bit;
        }
        else{
          break;
        }
      }
      else{
        break;
      }
    }
    for(j=0;j<i+1;j++){
      bit = uint64_set[diagonal_tables[1][i][j]];
      if(bit & input){
        if(bit & 0x0000000000000080){
          stable |= 0x0000000000000080;
        }
        else if(bit & 0x8080808080808000){
          stable |= (bit & (stable << 8));
        }
        else if(bit & (stable >> 1)){
          stable |= bit;
        }
        else{
          break;
        }
      }
      else{
        break;
      }
    }
    for(j=i;j>=0;j--){
      bit = uint64_set[diagonal_tables[1][i][j]];
      if(bit & input){
        if(bit & 0x0000000000000080){
          stable |= 0x0000000000000080;
        }
        else if(bit & 0x000000000000007F){
          stable |= (bit & (stable >> 1));
        }
        else if(bit & (stable << 8)){
          stable |= bit;
        }
        else{
          break;
        }
      }
      else{
        break;
      }
    }
    for(j=0;j<i+1;j++){
      bit = uint64_set[diagonal_tables[2][i][j]];
      if(bit & input){
        if(bit & 0x0100000000000000){
          stable |= 0x0100000000000000;
        }
        else if(bit & 0xFE00000000000000){
          stable |= (bit & (stable << 1));
        }
        else if(bit & (stable >> 8)){
          stable |= bit;
        }
        else{
          break;
        }
      }
      else{
        break;
      }
    }
    for(j=i;j>=0;j--){
      bit = uint64_set[diagonal_tables[2][i][j]];
      if(bit & input){
        if(bit & 0x0100000000000000){
          stable |= 0x0100000000000000;
        }
        else if(bit & 0x0001010101010101){
          stable |= (bit & (stable >> 8));
        }
        else if(bit & (stable << 1)){
          stable |= bit;
        }
        else{
          break;
        }
      }
      else{
        break;
      }
    }
    for(j=0;j<i+1;j++){
      bit = uint64_set[diagonal_tables[3][i][j]];
      if(bit & input){
        if(bit & 0x8000000000000000){
          stable |= 0x8000000000000000;
        }
        else if(bit & 0x7F00000000000000){
          stable |= (bit & (stable >> 1));
        }
        else if(bit & (stable >> 8)){
          stable |= bit;
        }
        else{
          break;
        }
      }
      else{
        break;
      }
    }
    for(j=i;j>=0;j--){
      bit = uint64_set[diagonal_tables[3][i][j]];
      if(bit & input){
        if(bit & 0x8000000000000000){
          stable |= 0x8000000000000000;
        }
        else if(bit & 0x0080808080808080){
          stable |= (bit & (stable >> 8));
        }
        else if(bit & (stable >> 1)){
          stable |= bit;
        }
        else{
          break;
        }
      }
      else{
        break;
      }
    }
  }
  
  return stable;
}

