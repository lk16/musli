#include <assert.h>
#include <stdio.h>
#include <malloc.h>

#include "board.h"
#include "macros.h"
#include "bot_moves.h"

void test_leak_new(void){
  malloc(5);
}



int main(){
  test_leak_new();
  return 0;
}
