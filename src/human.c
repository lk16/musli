#include "human.h"

int human_do_move(human* h, const board* b)
{
  (void)h;
  const int input_size = 4;
  char input[input_size];
  while(1){
    printf("%s","Enter a move: ");
    fgets(input,input_size,stdin);
    if(input[0]>='a' && input[0]<='h' && input[1]>='1' && input[1]<='8'){
      int move;
      move = (input[0]-'a') + 8*(input[1]-'1');
      if(board_is_valid_move(b,move)){
        return move;
      }
    }
    printf("%s","Invalid move.\n");
  }
}
