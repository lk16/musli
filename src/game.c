#include "game.h"

void game_reset(game* g)
{
  g->current = 0;
  g->redo_max = 0;
  board_init(&g->history[0].discs);
  g->history[0].turn = 0;
}

void game_init(game* g)
{
  game_reset(g);
  player_init(&g->players[0]);
  player_init(&g->players[1]);
}

int game_test_ended(game* g)
{
  {
    board* b = &g->history[g->current].discs;
    if(~(b->me | b->opp) == 0ull){
      return 1;
    }
    
  }
  
  
  g->current = g->redo_max = g->current + 1;
  
  
  game_state *gs,*prev;
  gs = g->history + g->current;
  prev = gs-1;
  
  gs->turn = prev->turn;
  
  if(board_has_valid_moves(&gs->discs)){
    gs->turn = 1 - gs->turn;
    return 0;
  }
  board_switch_turn(&gs->discs);
  if(board_has_valid_moves(&gs->discs)){
    gs->turn = 1 - gs->turn;
    return 0;
  }
  return 1;
}

void game_redo_move(game* g)
{
  if(g->current <= g->redo_max){
    g->current++;
  }
}

void game_undo_move(game* g)
{
  if(g->current >= 0){
    g->current--;
  }
}

void game_over(const game* g)
{
  (void)g;
  printf("%s","Game over!\n");
}

void game_play(game* g)
{
  do{
    player* p = g->players + g->history[g->current].turn;
    board_print(&g->history[g->current].discs,stdout,g->history[g->current].turn);
    player_do_move(p,&g->history[g->current].discs,&g->history[g->current+1].discs);
    
  }while(!game_test_ended(g));
  game_over(g);
}


