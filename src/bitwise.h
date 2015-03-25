#pragma once

#include <stdint.h>

#include "macros.h"

const uint64_t uint64_set[65];
const uint64_t uint64_reset[65];
const uint64_t uint64_before[65];
const uint64_t uint64_after[65];
const int uint64_counts[65536];


// thanks to http://www-cs-faculty.stanford.edu/~knuth/fasc1a.ps.gz
uint64_t uint64_mirror_vertical(uint64_t x);

// thanks to http://www-cs-faculty.stanford.edu/~knuth/fasc1a.ps.gz
uint64_t uint64_rotate_left(uint64_t x);

uint64_t uint64_rotate(uint64_t x,int n);


