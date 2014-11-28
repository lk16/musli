#pragma once

#include <stdint.h>

const uint64_t uint64_set[65];
const uint64_t uint64_reset[65];
const uint64_t uint64_before[65];
const uint64_t uint64_after[65];
const int uint64_counts[65536];

// returns 64 if b==0ul, returns least significant bit otherwise
inline int uint64_find_first(uint64_t b){
  if(b == 0ul){
    return 64;
  }
  return __builtin_ffsl(b) - 1;
}

// returns 64 if b==0ull, returns most significant bit otherwise
inline int uint64_find_last(uint64_t b){
  if(b == 0ul){
    // __builtin_clz(0ull) is undefined
    return 64;
  }
  return 63 - __builtin_clzl(b);  
}

// returns number of set bits in b
inline int uint64_count(uint64_t b){
#if 0
  return __builtin_popcountll(b);
#else
  return uint64_counts[b & 0xFFFF] + 
  uint64_counts[(b >> 16) & 0xFFFF] +
  uint64_counts[(b >> 32) & 0xFFFF] +
  uint64_counts[(b >> 48) & 0xFFFF];
#endif
}