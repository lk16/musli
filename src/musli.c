#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib-object.h>

#include "game.h"
#include "main_window.h"


void run_gui(int argc,char** argv){
  main_window mw;
  gtk_init(&argc,&argv);
  main_window_init(&mw);
  gtk_widget_show(mw.window);
  gtk_main();
}


int main(int argc,char** argv){
  if(argc>1 && strcmp(argv[1],"-nogui")!=0){
    game g;
    g.players[0].type = PLAYER_BOT_MOVES;
    g.players[1].type = PLAYER_BOT_MOVES;
    game_init(&g);
    player_set_level(&g.players[1],14,20);
    player_set_level(&g.players[0],14,20);
    game_play(&g);
    return 0;
  }
  run_gui(argc,argv);
  return 0;  
}