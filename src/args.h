#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "game_config.h"

enum{
  MUSLI_ARG_PARSE_ERROR = -1
};

typedef struct musli_parse_state{
  int argc,index;
  const char** argv;
} musli_parse_state;

typedef int(*musli_arg_func)(game_config*,const musli_parse_state*);

typedef struct musli_arg{
  const char* arg;
  musli_arg_func func;
} musli_arg;




void musli_arg_parse(int argc,const char** argv,game_config* gc);
void musli_arg_parse_show_error(const musli_parse_state* mps);

int musli_arg_parse_has_enough_args(const musli_parse_state* mps,int n);




int arg_set_black_level(game_config* gc,const musli_parse_state* mps);