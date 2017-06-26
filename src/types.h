#pragma once

#include <sys/types.h>

struct edge_t {
  uint32_t a, b, w;
};

struct graph_t {
  uint32_t edge_nbr;
  uint32_t k;
  struct edge_t *edges;
};
