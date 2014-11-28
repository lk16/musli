#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "player.h"

int main(){
  board b;
  board_init(&b);
  player p;
  p.type = PLAYER_HUMAN;
  while(1){  
    board_print(&b,stdout,0);
    int move = player_do_move(&p,&b);
    board_do_move(&b,move);
  }
  return 0;  
}
