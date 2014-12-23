#include "svg_helper.h"

void svg_print_header(FILE* f, int width, int height)
{
  fprintf(
    f,
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<svg xmlns=\"http://www.w3.org/2000/svg\" "
    "xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
    "height=\"%d\" width=\"%d\">\n<defs>"
    "<style type=\"text/css\">\n"
    "circle.w { fill: white; }\n"
    "circle.b { fill: black; }\n"
    "</style></defs>\n",
    height,
    width
  );
}

void svg_print_footer(FILE* f)
{
  fprintf(f,"%s","</svg>\n");
}

void svg_print_game_state(FILE* f, const struct game_state* gs,struct offset o)
{
  fprintf(
    f,
    "<rect x=\"%d\" y=\"%d\" width=\"%d\" "
    "height=\"%d\" fill=\"#34943F\" />\n",
    o.x,
    o.y,
    SVG_BOARD_WIDTH,
    SVG_BOARD_HEIGHT
  );
  
  uint64_t black,white;
  if(gs->turn){
    white = gs->discs.me;
    black = gs->discs.opp;
  }
  else{
    white = gs->discs.opp;
    black = gs->discs.me;
  }
  
  int i;
  const char* classname;
  for(i=0;i<64;i++){
    if(white & uint64_set[i]){
      classname = "w";
    }
    else if(black & uint64_set[i]){
      classname = "b";
    }
    else{
      continue;
    }
    fprintf(
      f,
      "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" class=\"%s\" />\n",
      o.x + (SVG_BOARD_WIDTH/16) + (SVG_BOARD_WIDTH/8)*(i%8),
      o.y + (SVG_BOARD_HEIGHT/16) + (SVG_BOARD_HEIGHT/8)*(i/8),
      SVG_BOARD_DISC_RADIUS,
      classname
    );
  }
}

struct svg_game_state_tree* svg_game_state_tree_generate(
  const struct game_state* gs,
  int depth
){
  struct svg_game_state_tree* res;
  res = malloc(sizeof(struct svg_game_state_tree));
  res->gs = *gs;
  if(depth==0){
    res->child_count = 0;
    return res;
  }
  struct game_state children[32],*child_end,*child_it;
  child_end = game_state_get_children(gs,children);
  res->child_count = child_end - children;
  struct svg_game_state_tree** res_child_it = res->children;
  for(child_it=children;child_it!=child_end;child_it++){
    *res_child_it = svg_game_state_tree_generate(child_it,depth-1);
    res_child_it++;
  }
  return res;
}

void svg_game_state_tree_free(struct svg_game_state_tree* tree)
{
  int i;
  for(i=0;i<tree->child_count;i++){
    svg_game_state_tree_free(tree->children[i]);
  }
  free(tree);
}

void svg_game_state_tree_get_width_table(
  const struct svg_game_state_tree* root,
  int* table,
  int max_depth
){
  int i;
  for(i=0;i<=max_depth;i++){
    table[i] = 0;
  }
  svg_game_state_tree_get_width_table_internal(root,0,table);
}

void svg_game_state_tree_get_width_table_internal(
  const struct svg_game_state_tree* tree,
  int depth,
  int* table
){
  table[depth]++;
  int i;
  for(i=0;i<tree->child_count;i++){
    svg_game_state_tree_get_width_table_internal(tree->children[i],depth+1,table);
  }
}

void svg_game_state_tree_print(
  FILE* f,
  const struct svg_game_state_tree* tree,
  int max_depth
){
  int* width_table = malloc(max_depth*sizeof(int));
  svg_game_state_tree_get_width_table(tree,width_table,max_depth);
  int max_width=0;
  int i;
  for(i=0;i<=max_depth;i++){
    if(width_table[i]>max_width){
      max_width = width_table[i];
    }
  }
  
  int* max_x_per_depth = malloc((max_depth+1)*sizeof(int));
  for(i=0;i<=max_width;i++){
    max_x_per_depth[i] = SVG_BOARD_X_SPACE ;
  }
  
  svg_print_header(
    f,
    SVG_BOARD_X_SPACE+((SVG_BOARD_X_SPACE+SVG_BOARD_WIDTH)*max_width),
    SVG_BOARD_Y_SPACE+((SVG_BOARD_Y_SPACE+SVG_BOARD_HEIGHT)*(max_depth+1))
  );
  svg_game_state_tree_print_internal(f,tree,0,max_depth,max_x_per_depth);
  svg_print_footer(f);
  free(width_table);
//  free(max_x_per_depth);
}

// returns x
int svg_game_state_tree_print_internal(
  FILE* f,
  const struct svg_game_state_tree* tree,
  int depth,
  int max_depth,
  int* max_used_x_per_depth
){
  struct offset o;
  o.y = SVG_BOARD_Y_SPACE + (depth*(SVG_BOARD_Y_SPACE + SVG_BOARD_HEIGHT));
  
  if(tree->child_count==0){
    o.x = max_used_x_per_depth[depth];
    max_used_x_per_depth[depth] += SVG_BOARD_X_SPACE + SVG_BOARD_WIDTH;
    svg_print_game_state(f,&tree->gs,o);
    return o.x;
  }
  int total = 0;
  int i;
  
  int* child_x_values = malloc(tree->child_count*sizeof(int));
  
  for(i=0;i<tree->child_count;i++){
    child_x_values[i] = svg_game_state_tree_print_internal(f,tree->children[i],
                                                depth+1,max_depth,max_used_x_per_depth);
    total += child_x_values[i];
  }
  
  o.x = total / tree->child_count;
  max_used_x_per_depth[depth] = o.x;
  
  
  for(i=0;i<tree->child_count;i++){
    fprintf(
      f,
      "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n",
      o.x + (SVG_BOARD_WIDTH/2),
      o.y + SVG_BOARD_HEIGHT,
      child_x_values[i] + (SVG_BOARD_WIDTH/2),
      o.y + SVG_BOARD_HEIGHT + SVG_BOARD_Y_SPACE 
    );
  }
  
  free(child_x_values);
  
  svg_print_game_state(f,&tree->gs,o);
  return o.x;
}
