#include "bot_random.h"

void bot_random_init(struct bot_random* bot)
{
  (void)bot;
  srand(time(NULL));
}

void bot_random_do_move(struct bot_random* bot,const struct board* b,struct board* res)
{
  (void)bot;
  struct board children[32],*child_end;
  child_end = board_get_children(b,children);
  *res = children[rand() % (child_end-children)];
}

