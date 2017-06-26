#pragma once

#include <cmath>
#include <sys/types.h>

struct edge_t {
  uint32_t a, b;
  float w;
};

struct node_t {
  uint32_t id;
  float x, y;
};

struct graph_t {
  uint32_t edge_nbr;
  uint32_t k;
  struct node_t *nodes;
  struct edge_t *edges;
};
