#pragma once

#include <stdio.h>

#include "board.h"

struct human{
};

void human_do_move(struct human* h,const struct board* b,struct board* res);