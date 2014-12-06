#include "args.h"

static const musli_arg musli_arg_list[] = {
  {"-lb",arg_set_black_level},
  {"-lw",arg_set_white_level}
};

void arg_parse(int argc, const char** argv, game_config* gc)
{
  const int musli_arg_list_size = sizeof(musli_arg_list)/sizeof(musli_arg_list[0]);
  parse_state ps;
  ps.argc = argc;
  ps.argv = argv;
  ps.index = 1;
  while(ps.index < argc){
    int i;
    int found = 0;
    for(i=0;i<musli_arg_list_size;i++){
      if(!strcmp(musli_arg_list[i].arg,ps.argv[ps.index])){
        found = 1;
        break;
      }
    }
    if(!found){
      arg_parse_show_error(&ps);
      exit(1);
    }
    int ret = musli_arg_list[i].func(gc,&ps);
    if(ret == MUSLI_ARG_PARSE_ERROR){
      arg_parse_show_error(&ps);
      exit(1);
    }
    ps.index += ret;
  }
}

int arg_parse_has_enough_args(const parse_state* ps, int n)
{
  return ps->index + n <= ps->argc;
}

int arg_set_level(game_config* gc, const parse_state* ps, int colour)
{
  int d,pd;
  d = pd = 0;
  if((arg_parse_has_enough_args(ps,3))
    && (sscanf(ps->argv[ps->index + 1],"%d",&d) == 1)
    && (sscanf(ps->argv[ps->index + 2],"%d",&pd) == 1)
  ){
    if(gc->players[colour].type == PLAYER_HUMAN){
      player_init(&gc->players[colour],PLAYER_DEFAULT_BOT,d,pd);
    }
    else{
      player_set_level(&gc->players[colour],d,pd);
    }
    return 3;
  }
  return MUSLI_ARG_PARSE_ERROR;
}


int arg_set_black_level(game_config* gc, const parse_state* ps)
{
  return arg_set_level(gc,ps,0);
}

int arg_set_white_level(game_config* gc, const parse_state* ps)
{
  return arg_set_level(gc,ps,1);
}



void arg_parse_show_error(const parse_state* mps)
{
  printf("Error in parsing near argument \"%s\".\n",mps->argv[mps->index]);
}
