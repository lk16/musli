#pragma once

#include <time.h>

#include "board.h"

struct bot_random{
};

void bot_random_init(struct bot_random* bot);

void bot_random_do_move(struct bot_random* bot,const struct board* b,struct board* res);

