#include "args.h"

static const musli_arg musli_arg_list[] = {
  {"-lb",arg_set_black_level}
};

void musli_arg_parse(int argc, const char** argv, game_config* gc)
{
  const int musli_arg_list_size = sizeof(musli_arg_list)/sizeof(musli_arg_list[0]);
  musli_parse_state mps;
  mps.argc = argc;
  mps.argv = argv;
  mps.index = 1;
  while(mps.index < argc){
    int i;
    int found = 0;
    for(i=0;i<musli_arg_list_size;i++){
      if(!strcmp(musli_arg_list[i].arg,mps.argv[mps.index])){
        found = 1;
        break;
      }
    }
    if(!found){
      musli_arg_parse_show_error(&mps);
      exit(1);
    }
    int ret = musli_arg_list[i].func(gc,&mps);
    if(ret == MUSLI_ARG_PARSE_ERROR){
      musli_arg_parse_show_error(&mps);
      exit(1);
    }
    mps.index += ret;
  }
}

int musli_arg_parse_has_enough_args(const musli_parse_state* mps, int n)
{
  return mps->index + n <= mps->argc;
}



int arg_set_black_level(game_config* gc, const musli_parse_state* mps)
{
  int d,pd;
  d = pd = 0;
  if((musli_arg_parse_has_enough_args(mps,3))
    && (sscanf(mps->argv[mps->index + 1],"%d",&d) == 1)
    && (sscanf(mps->argv[mps->index + 2],"%d",&pd) == 1)
  ){
    if(gc->players[0].type == PLAYER_HUMAN){
      player_init(&gc->players[0],PLAYER_DEFAULT_BOT,d,pd);
    }
    else{
      player_set_level(&gc->players[0],d,pd);
    }
    return 3;
  }
  return MUSLI_ARG_PARSE_ERROR;
}

void musli_arg_parse_show_error(const musli_parse_state* mps)
{
  printf("Error in parsing near argument \"%s\".\n",mps->argv[mps->index]);
}
