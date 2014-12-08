#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "game_config.h"
#include "player_name.h"

enum{
  MUSLI_ARG_PARSE_ERROR = -1
};

struct parse_state{
  int argc,index;
  const char** argv;
};

typedef int(*musli_arg_func)(struct game_config*,const struct parse_state*);

struct musli_arg{
  const char* arg;
  musli_arg_func func;
};




// main arg parse implementation
void arg_parse(int argc,const char** argv,struct game_config* gc);
void arg_parse_show_error(const struct parse_state* ps);
int arg_parse_has_enough_args(const struct parse_state* ps,int n);

// arg parse func helpers
int arg_set_level(struct game_config* gc,const struct parse_state* ps,int color);

// arg parse funcs
int arg_set_black_level(struct game_config* gc,const struct parse_state* ps);
int arg_set_white_level(struct game_config* gc,const struct parse_state* ps);
int arg_set_bot_type(struct game_config* gc,const struct parse_state* ps);
int arg_set_type_setup(struct game_config* gc,const struct parse_state* ps);