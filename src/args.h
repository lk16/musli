#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "game_config.h"

enum{
  MUSLI_ARG_PARSE_ERROR = -1
};

typedef struct parse_state{
  int argc,index;
  const char** argv;
} parse_state;

typedef int(*musli_arg_func)(game_config*,const parse_state*);

typedef struct musli_arg{
  const char* arg;
  musli_arg_func func;
} musli_arg;





void arg_parse(int argc,const char** argv,game_config* gc);
void arg_parse_show_error(const parse_state* ps);

int arg_parse_has_enough_args(const parse_state* ps,int n);

int arg_set_level(game_config* gc,const parse_state* ps,int color);
int arg_set_black_level(game_config* gc,const parse_state* ps);
int arg_set_white_level(game_config* gc,const parse_state* ps);

int arg_set_bot_type(game_config* gc,const parse_state* ps);
