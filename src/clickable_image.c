#include "clickable_image.h"

void clickable_image_init(
  struct clickable_image* ci, 
  const char* file,
  int index,
  struct game_config* gc
){
  ci->gc = gc;
  ci->index = index;  
  ci->ebox = gtk_event_box_new();
  ci->image = gtk_image_new_from_file(file);
  gtk_container_add(GTK_CONTAINER(ci->ebox),ci->image);
  g_signal_connect(ci->ebox,"button-press-event",
    G_CALLBACK(clickable_image_on_click),
    ci
  );
}

void game_config_process_click(struct game_config*,int,int);

void clickable_image_on_click(
  GtkWidget* widget,
  GdkEvent* event,
  struct clickable_image* ci
){
  (void)widget;
  if(event->button.type==GDK_BUTTON_PRESS && 
    (event->button.button==1 || event->button.button==3)
  ){
    game_config_process_click(ci->gc,ci->index,event->button.button);
  }
}
