#pragma once

#include "types.h"

struct gpu_settings_t {
  int per_ker_width;
  uint32_t k;
  uint32_t edge_nbr;
};

int greedy_gpu(struct graph_t *graph, struct graph_t *output, float t);
