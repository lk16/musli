#pragma once

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

#include "pvs_helper.h"
#include "game_config.h"

#define SVG_BOARD_WIDTH (80)
#define SVG_BOARD_HEIGHT (SVG_BOARD_WIDTH)
#define SVG_BOARD_X_SPACE (10)
#define SVG_BOARD_Y_SPACE (50)
#define SVG_BOARD_DISC_RADIUS (4)


struct offset{
  int x,y;
};

struct svg_game_state_tree{
  struct game_state gs;
  struct svg_game_state_tree* children[32];
  int child_count;  
};

void svg_print_header(FILE* f,int height,int width);
void svg_print_footer(FILE* f);

void svg_print_game_state(FILE* f,const struct game_state* gs,struct offset o);

struct svg_game_state_tree* svg_game_state_tree_generate_full(
  const struct game_state* gs,
  int depth
);

struct svg_game_state_tree* svg_game_state_tree_generate_best(
  const struct game_state* gs,
  int depth,
  int search_depth,
  int(*heur)(const struct board*),
  FILE* out,
  int colour
);

void svg_game_state_tree_print(
  FILE* f,
  const struct svg_game_state_tree* tree,
  int max_depth
);

// returns x
int svg_game_state_tree_print_internal(
  FILE* f,
  const struct svg_game_state_tree* tree,
  int depth,
  int max_depth,
  int* max_used_x_per_depth
);

void svg_game_state_tree_free(
  struct svg_game_state_tree* tree
);

void svg_game_state_tree_get_width_table(
  const struct svg_game_state_tree* root,
  int* table,
  int max_depth
);

void svg_game_state_tree_get_width_table_internal(
  const struct svg_game_state_tree* tree,
  int depth,
  int* table
);
