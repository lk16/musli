#pragma once

#include "player.h"

struct player_name_table_row{
  const char* name;
  enum player_type type;
};

extern const struct player_name_table_row player_name_table[];

enum player_type player_name_to_type(const char* name);
const char* player_type_to_name(enum player_type type);