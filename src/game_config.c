#include "game_config.h"

void game_config_on_new_game(struct game_config* gc)
{
  gc->current = gc->redo_max = 0;
  game_state_init(&gc->history[0]);
  game_config_show_updated_field(gc);
}

void game_config_init(struct game_config* gc,struct main_window* mw)
{
  player_init(&gc->players[0],PLAYER_HUMAN,1,1);
  player_init(&gc->players[1],PLAYER_HUMAN,1,1);
  gc->type = GAME_TYPE_MATCH;
  gc->display = DISPLAY_WINDOW;
  gc->window = mw;
  gc->bot_type = PLAYER_DEFAULT_BOT;
  game_config_on_new_game(gc);
}

void game_config_show_updated_field(const struct game_config* gc)
{
  const struct game_state* current = game_config_get_state_const(gc);
  
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

void game_config_process_click(struct game_config* gc, int index,int button)
{
  struct player* player_to_move = game_config_get_player_to_move(gc);
  struct game_state* state = game_config_get_state(gc);
  
  switch(gc->type){
    case GAME_TYPE_MATCH:{
      if(button == RIGHT_BUTTON){
        game_config_undo_move(gc);
        break;
      }
      if(player_to_move->type != PLAYER_HUMAN){
        return;
      }
      if(!board_is_valid_move(&state->discs,index)){
        return;
      }
      struct board child = state->discs;
      board_do_move(&child,index);
      game_config_on_any_move(gc,&child);
    }
    break;
    case GAME_TYPE_SETUP:{
      switch(button){
        case LEFT_BUTTON:{
          uint64_t *me = &state->discs.me;
          uint64_t *opp = &state->discs.opp;
          if(*me & uint64_set[index]){
            *me &= uint64_reset[index];
            *opp |= uint64_set[index];
          }
          else if(*opp & uint64_set[index]){
            *opp &= uint64_reset[index];
          }
          else{
            *me |= uint64_set[index];
          }
        }
        break;
        case RIGHT_BUTTON:{
          game_state_switch_turn(state);
        }
        break;
      }
    }
    break;
    default:
      printf("%s","game_config_process_click: this is not implemented!\n");
  }
  game_config_show_updated_field(gc);
}
    

struct player* game_config_get_player_to_move(struct game_config* gc)
{
  return &gc->players[gc->history[gc->current].turn];
}

int game_config_timeout(struct game_config* gc)
{
  if(gc->type == GAME_TYPE_MATCH){
    struct player* p = game_config_get_player_to_move(gc);
    if(p->type != PLAYER_HUMAN){
      struct game_state* s = game_config_get_state(gc);
      if(!board_test_game_ended(&s->discs)){
        struct board child;
        player_do_move(p,&s->discs,&child);
        game_config_on_any_move(gc,&child);
      }
    }
  }
  return G_SOURCE_CONTINUE;
}

void game_config_on_any_move(struct game_config* gc,const struct board* child)
{
  struct game_state* s = game_config_get_state(gc);
  (s+1)->discs = *child;
  (s+1)->turn = s->turn; 
  game_state_update_turn(s+1);
  gc->current = gc->redo_max = gc->current + 1;
  game_config_show_updated_field(gc);
  if(gc->players[0].type!=PLAYER_HUMAN && gc->players[1].type!=PLAYER_HUMAN){
    game_state_print(s+1,stdout);
  }
  if(board_test_game_ended(&(s+1)->discs)){
    game_config_on_ended(gc);
  }
}



void game_config_redo_move(struct game_config* gc)
{
  int cur = gc->current + 1;
  while(cur < gc->redo_max){
    if(gc->players[gc->history[cur].turn].type == PLAYER_HUMAN){
      gc->current = cur;
      return;
    }
    cur++;
  }
}

void game_config_undo_move(struct game_config* gc)
{
  int cur = gc->current -1;
  while(cur >= 0){
    if(gc->players[gc->history[cur].turn].type == PLAYER_HUMAN){
      gc->current = cur;
      return;
    }
    cur--;
  }
}

void game_config_on_ended(const struct game_config* gc)
{
  printf("%s","Game over!\n");
  const struct game_state* s = game_config_get_state_const(gc);
  int count[2];
  count[0] = uint64_count(s->turn ? s->discs.opp : s->discs.me);
  count[1] = uint64_count(s->turn ? s->discs.me : s->discs.opp);
  if(count[0] > count[1]){
    printf("Black wins: %d - %d\n",count[0],count[1]);
  }
  else if(count[0] < count[1]){
    printf("White wins: %d - %d\n",count[1],count[0]);
  }
  else{
    printf("Draw: %d - %d\n",count[0],count[1]);
  }
}

struct game_state* game_config_get_state(struct game_config* gc)
{
  return &gc->history[gc->current];
}

const struct game_state* game_config_get_state_const(const struct game_config* gc)
{
  return &gc->history[gc->current];
}



void game_config_console_main(struct game_config* gc)
{
  struct game_state *state,*next;
  
  while(1){
    state = game_config_get_state(gc);
    if(board_test_game_ended(&state->discs)){
      break;
    }
    next = state + 1;
    struct player* p = gc->players + state->turn;
    game_config_show_updated_field(gc);
    player_do_move(p,&state->discs,&next->discs);
    gc->current = gc->redo_max = gc->current + 1;
    game_state_update_turn(next);
  }
  game_config_on_ended(gc);
}


void game_state_print(const struct game_state* gs, FILE* file)
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

void game_state_init(struct game_state* gs)
{
  board_init(&gs->discs);
  gs->turn = 0;
}

void game_state_update_turn(struct game_state* gs)
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

void game_state_switch_turn(struct game_state* gs)
{
  board_switch_turn(&gs->discs);
  gs->turn = 1 - gs->turn;
}

void game_config_set_bot(struct game_config* gc, int colour, int depth, int perfect_depth)
{
  assert(colour==0 || colour==1);
  player_init(&gc->players[colour],gc->bot_type,depth,perfect_depth);
}

struct game_state* game_state_get_children(const struct game_state* gs,struct game_state* out)
{
  struct board children[32];
  struct board* end = board_get_children(&gs->discs,children);
  if(end==children){
    struct game_state copy = *gs;
    game_state_switch_turn(&copy);
    if(board_has_valid_moves(&copy.discs)){
      return game_state_get_children(&copy,out);
    }
    return out;
  }
  struct game_state* out_end = out;
  struct board* it;
  for(it=children;it!=end;it++){
    out_end->discs = *it;
    out_end->turn = 1 - gs->turn;
    out_end++;
  }
  return out_end;
}


