#include "bot_stats.h"

void bot_stats_init(bot_stats* s)
{
  s->nodes = 0ull;
}

void bot_stats_start(bot_stats* s)
{
  s->nodes = 0ull;
  gettimeofday(&s->start_time,NULL);
}

void bot_stats_stop(bot_stats* s)
{
  gettimeofday(&s->stop_time,NULL);
}

double bot_stats_get_seconds(const bot_stats* s)
{
  return (s->stop_time.tv_sec + (s->stop_time.tv_usec / 1000000.0)) - 
  (s->start_time.tv_sec + (s->start_time.tv_usec / 1000000.0)); 
}

long long unsigned int bot_stats_get_nodes_per_second(const bot_stats* s)
{
  if(bot_stats_get_seconds(s) < 0.00000001){
    return 0ull;
  }
  return (unsigned long long)(s->nodes / bot_stats_get_seconds(s));
}