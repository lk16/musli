#pragma once

#include "assert.h"

#define MIN_HEURISTIC (-64000)
#define MAX_HEURISTIC (64000)
#define MIN_PERFECT_HEURISTIC (-64)
#define MAX_PERFECT_HEURISTIC (64)


// returns 64 if b==0ul, returns least significant bit otherwise
#define uint64_find_first(b) \
  ((b) == 0ul) ? 64 : (__builtin_ffsl(b) - 1) 

// returns 64 if b==0ull, returns most significant bit otherwise
#define uint64_find_last(b) \
  (((b) == 0ul) ? 64 : (63 - __builtin_clzl(b))) 

// returns number of set bits
#define uint64_count(b) \
  ( \
    uint64_counts[(b) & 0xFFFF] + \
    uint64_counts[((b) >> 16) & 0xFFFF] + \
    uint64_counts[((b) >> 32) & 0xFFFF] + \
    uint64_counts[((b) >> 48) & 0xFFFF] \
  )

#define generic_swap(x,y) \
  { __typeof__(x) __tmp = x; x=y; y=__tmp; } 
  