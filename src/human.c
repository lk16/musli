#include "human.h"

void human_do_move(struct human* h, const struct board* b,struct board* res)
{
  (void)h;
  const int input_size = 4;
  char input[input_size];
  board_print(b,stdout,0);
  while(1){
    printf("%s","Enter a move: ");
    fgets(input,input_size,stdin);
    if(input[0]>='a' && input[0]<='h' && input[1]>='1' && input[1]<='8'){
      int move;
      move = (input[0]-'a') + 8*(input[1]-'1');
      if(board_is_valid_move(b,move)){
        *res = *b;
        board_do_move(res,move);
        return;
      }
    }
    printf("%s","Invalid move.\n");
    fflush(stdin);
  }
}
