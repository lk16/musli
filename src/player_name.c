#include "player_name.h"

const struct player_name_table_row player_name_table[] = {
  {"moves",PLAYER_BOT_MOVES},
  {"random",PLAYER_BOT_RANDOM},
  {"least",PLAYER_BOT_LEAST},
  {"stable",PLAYER_BOT_STABLE},
  {"most",PLAYER_BOT_MOST}
};

enum player_type player_name_to_type(const char* name)
{
  int n_rows = sizeof(player_name_table)/sizeof(player_name_table[0]);
  int i;
  for(i=0;i<n_rows;i++){
    if(!strcmp(name,player_name_table[i].name)){
      return player_name_table[i].type;
    }
  }
  return PLAYER_NOT_FOUND;
}

const char* player_type_to_name(enum player_type type)
{
  int n_rows = sizeof(player_name_table)/sizeof(player_name_table[0]);
  int i;
  for(i=0;i<n_rows;i++){
    if(player_name_table[i].type == type){
      return player_name_table[i].name;
    }
  }
  return "player not found";
}

