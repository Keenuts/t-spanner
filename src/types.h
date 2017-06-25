#pragma once

#include <sys/types.h>

struct edge_t {
  int a, b, w;
};

struct graph_t {
  uint64_t edge_nbr;
  struct edge_t *edges;
};
