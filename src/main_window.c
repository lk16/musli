#include "main_window.h"

void main_window_init(struct main_window* mw)
{
  
  mw->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(mw->window),"Musli");
  gtk_window_set_default_size(GTK_WINDOW(mw->window),400,400);
  g_signal_connect(mw->window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
  
  mw->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  
  mw->table = gtk_table_new(8,8,FALSE);
  
  main_window_table_init(mw);
  game_config_init(&mw->config,mw);
  
  gtk_box_pack_start(GTK_BOX(mw->vbox),mw->table,TRUE,0,0);
  
  gtk_container_add(GTK_CONTAINER(mw->window),mw->vbox);
  
  gtk_widget_show_all(mw->window);
}

void main_window_table_init(struct main_window* mw)
{
  int x,y;
  for(x=0;x<8;x++){
    for(y=0;y<8;y++){
      clickable_image_init(&mw->images[x][y],"./images/empty.png",y*8+x,&mw->config);
      gtk_table_attach(GTK_TABLE(mw->table),mw->images[x][y].ebox,
        x,x+1,
        y,y+1,
        GTK_SHRINK,GTK_SHRINK,
        0,0
      );
    }
  }
}


void main_window_update_fields(struct main_window* mw, const struct game_state* gs)
{
  const char* imagefile;
  uint64_t black = (gs->turn ? gs->discs.opp : gs->discs.me);
  uint64_t white = (gs->turn ? gs->discs.me : gs->discs.opp);
  
  
  int i;
  for(i=0;i<64;i++){
    if(white & uint64_set[i]){
      imagefile = "./images/white.png";
    }
    else if(black & uint64_set[i]){
      imagefile = "./images/black.png";
    }
    else if(board_is_valid_move(&gs->discs,i)){
      if(gs->turn){
        imagefile = "./images/move_white.png";
      }
      else{
        imagefile = "./images/move_black.png";
      }
    }
    else{
      imagefile = "./images/empty.png";
    }
    GtkWidget* im = mw->images[i%8][i/8].image;
    gtk_image_set_from_file(GTK_IMAGE(im),imagefile);
  }
}



