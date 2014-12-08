#pragma once

#include <stddef.h>
#include <sys/time.h>



struct bot_stats{
  struct timeval start_time,stop_time;
  unsigned long long nodes;
};

void bot_stats_init(struct bot_stats* s);
void bot_stats_start(struct bot_stats* s);
void bot_stats_stop(struct bot_stats* s);
double bot_stats_get_seconds(const struct bot_stats* s);
unsigned long long bot_stats_get_nodes_per_second(const struct bot_stats* s);
