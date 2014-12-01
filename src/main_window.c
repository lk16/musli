#include "main_window.h"

void main_window_init(main_window* mw)
{
  game_config_init(&mw->config);
  
  mw->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(mw->window),"Musli");
  gtk_window_set_default_size(GTK_WINDOW(mw->window),400,400);
  g_signal_connect(mw->window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
  
  mw->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  
  mw->table = gtk_table_new(8,8,FALSE);
  
  int x,y;
  for(x=0;x<8;x++){
    for(y=0;y<8;y++){
      GtkWidget* ebox = gtk_event_box_new();
      gtk_table_attach(GTK_TABLE(mw->table),ebox,
        x,x+1,
        y,y+1,
        GTK_SHRINK,GTK_SHRINK,
        0,0
      );
     GtkWidget* image = gtk_image_new_from_file("./images/empty.png");
     gtk_container_add(GTK_CONTAINER(ebox),image);
    }
  }
  gtk_box_pack_start(GTK_BOX(mw->vbox),mw->table,TRUE,0,0);
  
  gtk_container_add(GTK_CONTAINER(mw->window),mw->vbox);
  
  gtk_widget_show_all(mw->window);
}
