#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib-object.h>

#include "game_config.h"
#include "main_window.h"
#include "args.h"



int main(int argc,char** argv){
  main_window mw;
  gtk_init(&argc,&argv);
  main_window_init(&mw);  
  musli_arg_parse(argc,(const char**)argv,&mw.config);
  g_timeout_add(100,(GSourceFunc)game_config_timeout,&mw.config);
  gtk_main();
  return 0;  
}