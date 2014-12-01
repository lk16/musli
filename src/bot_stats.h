#pragma once

#include <stddef.h>
#include <sys/time.h>

typedef struct bot_stats{
  struct timeval start_time,stop_time;
  unsigned long long nodes;
} bot_stats;

void bot_stats_init(bot_stats* s);
void bot_stats_start(bot_stats* s);
void bot_stats_stop(bot_stats* s);
double bot_stats_get_seconds(const bot_stats* s);
unsigned long long bot_stats_get_nodes_per_second(const bot_stats* s);
