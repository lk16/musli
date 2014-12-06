#include "game_config.h"

void game_config_on_new_game(game_config* gc)
{
  gc->current = gc->redo_max = 0;
  game_state_init(&gc->history[0]);
  game_config_show_updated_field(gc);
}

void game_config_init(game_config* gc,struct main_window* mw)
{
  player_init(&gc->players[0],PLAYER_HUMAN,1,1);
  player_init(&gc->players[1],PLAYER_HUMAN,1,1);
  gc->type = GAME_TYPE_MATCH;
  gc->display = DISPLAY_WINDOW;
  gc->window = mw;
  game_config_on_new_game(gc);
}

void main_window_update_fields(struct main_window*,const game_state*);

void game_config_show_updated_field(const game_config* gc)
{
  const game_state* current = game_config_get_state_const(gc);
  
  switch(gc->display){
    case DISPLAY_CONSOLE:
      game_state_print(current,stdout);
      break;
    case DISPLAY_WINDOW:
      main_window_update_fields(gc->window,current);
      break;
    default:
      printf("%s","Not implemented.\n"); 
  }
}

void game_config_process_click(game_config* gc, int index)
{
  switch(gc->type){
    case GAME_TYPE_MATCH:{
      player* p = game_config_get_player_to_move(gc);
      if(p->type != PLAYER_HUMAN){
        return;
      }
      game_state* state = game_config_get_state(gc);
      if(!board_is_valid_move(&state->discs,index)){
        return;
      }
      
      *(state+1) = *state;
      board_do_move(&((state+1)->discs),index);
      game_state_update_turn(state+1);
      gc->current = gc->redo_max = gc->current + 1;
      game_config_show_updated_field(gc);
      break;
    }
    default:
      printf("%s","game_config_process_click: this is not implemented!\n");
  }
}

player* game_config_get_player_to_move(game_config* gc)
{
  return &gc->players[gc->history[gc->current].turn];
}

int game_config_timeout(game_config* gc)
{
  if(gc->type == GAME_TYPE_MATCH){
    player* p = game_config_get_player_to_move(gc);
    if(p->type != PLAYER_HUMAN){
      game_state* s = game_config_get_state(gc);
      if(!board_test_game_ended(&s->discs)){
        player_do_move(p,&s->discs,&((s+1)->discs));
        (s+1)->turn = s->turn; 
        game_state_update_turn(s+1);
        gc->current = gc->redo_max = gc->current + 1;
        game_config_show_updated_field(gc);
      }
    }
  }
  return G_SOURCE_CONTINUE;
}



void game_config_redo_move(game_config* gc)
{
  if(gc->current <= gc->redo_max){
    gc->current++;
  }
}

void game_config_undo_move(game_config* gc)
{
  if(gc->current >= 0){
    gc->current--;
  }
}

void game_config_on_ended(const game_config* gc)
{
  printf("%s","Game over!\n");
  const game_state* s = game_config_get_state_const(gc);
  int count[2];
  count[0] = s->turn ? s->discs.opp : s->discs.me;
  count[1] = s->turn ? s->discs.me : s->discs.opp;
  if(count[0] > count[1]){
    printf("Black wins: %d - %d\n",count[0],count[1]);
  }
  else if(count[0] < count[1]){
    printf("White wins: %d - %d\n",count[1],count[0]);
  }
  else{
    printf("Draw: %d - %d",count[0],count[1]);
  }
}

game_state* game_config_get_state(game_config* gc)
{
  return &gc->history[gc->current];
}

const game_state* game_config_get_state_const(const game_config* gc)
{
  return &gc->history[gc->current];
}



void game_config_console_main(game_config* gc)
{
  game_state *state,*next;
  
  while(1){
    state = game_config_get_state(gc);
    next = state + 1;
    player* p = gc->players + state->turn;
    game_config_show_updated_field(gc);
    player_do_move(p,&state->discs,&next->discs);
    gc->current = gc->redo_max = gc->current + 1;
    next->turn = 1 - state->turn;
    if(board_has_valid_moves(&next->discs)){
      continue;
    }
    //board_switch_turn(&next->discs);
    next->turn = 1 - next->turn;
    if(board_has_valid_moves(&next->discs)){
      continue;
    }
    break;
  }
  game_config_on_ended(gc);
}


void game_state_print(const game_state* gs, FILE* file)
{
  fprintf(file,"%s","+-a-b-c-d-e-f-g-h-+\n");
  int f;
  uint64_t moves,white,black;
  moves = board_get_moves(&gs->discs);
  black = gs->turn ? gs->discs.me : gs->discs.opp;
  white = gs->turn ? gs->discs.opp : gs->discs.me;
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

void game_state_init(game_state* gs)
{
  board_init(&gs->discs);
  gs->turn = 0;
}

void game_state_update_turn(game_state* gs)
{
  gs->turn = 1 - gs->turn;
  if(board_has_valid_moves(&gs->discs)){
    return;
  }
  game_state_switch_turn(gs);
  if(board_has_valid_moves(&gs->discs)){
    return;
  }
  game_state_switch_turn(gs); 
}

void game_state_switch_turn(game_state* gs)
{
  board_switch_turn(&gs->discs);
  gs->turn = 1 - gs->turn;
}




